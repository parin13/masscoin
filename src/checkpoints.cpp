// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
         (0, uint256("0xeee7f58d111ef0b25f7f6e8703d80ab6b51ca957170daea949e93e6467906889"))
         (2, uint256("0xc867ddc3b8200267229c8c98c363dcbfd67dc637c54b450d7a221acfd592c950"))
         (8, uint256("0x8f8faffdaa22340da242f6e2410bb76780d5d79a158884b82c5fbeabd07d09bf"))
         (16, uint256("0xef9f119822ae646903e57514423148b7b76a4e34f982ce75fb404f51fbf8c8df"))
         (150, uint256("0x2274364a06434d8be83206a750a4640ea778971f98308950609dfd7d74bb9345"))
         (500, uint256("0x34f244f47f3a05354731febc29ce9d42099e187231098ed5069ea0189b287871"))
         (4000, uint256("0x477312f3f2c399309fc754f04d56702ff914e0db4db2f1899dd75302bc8acd35"))
         (40000, uint256("f101b3f269639c9c3a8211462f51d9d1aa61be483780c2b6418c5b3b29dcbf07"))
         // (161500, uint256("0xdbe89880474f4bb4f75c227c77ba1cdc024991123b28b8418dbbf7798471ff43"))
         // (179620, uint256("0x2ad9c65c990ac00426d18e446e0fd7be2ffa69e9a7dcb28358a50b2b78b9f709"))
         // (240000, uint256("0x7140d1c4b4c2157ca217ee7636f24c9c73db39c4590c4e6eab2e3ea1555088aa"))
         // (383640, uint256("0x2b6809f094a9215bafc65eb3f110a35127a34be94b7d0590a096c3f126c6f364"))
         // (409004, uint256("0x487518d663d9f1fa08611d9395ad74d982b667fbdc0e77e9cf39b4f1355908a3"))
         // (456000, uint256("0xbf34f71cc6366cd487930d06be22f897e34ca6a40501ac7d401be32456372004"))
         // (541794, uint256("0x1cbccbe6920e7c258bbce1f26211084efb19764aa3224bec3f4320d77d6a2fd2"))
         // (585010, uint256("0xea9ea06840de20a18a66acb07c9102ee6374ad2cbafc71794e576354fea5df2d"))
         // (638902, uint256("0x15238656e8ec63d28de29a8c75fcf3a5819afc953dcd9cc45cecc53baec74f38"))

        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1518520240, // * UNIX timestamp of last checkpoint block
         0,    // * total number of transactions between genesis and last checkpoint
        //             //   (the tx=... number in the SetBestChain debug.log lines)
         4000.0     // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        // (   546, uint256("0xa0fea99a6897f531600c8ae53367b126824fd6a847b2b2b73817a95b8e27e602"))
        (   0, uint256("0xe0d8c255be8de843e56b2b31c65126e7732da28504046437077173b01b7cb222"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
         1518520229,
         0,
          4000.0
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
