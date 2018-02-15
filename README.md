Masscoin integration/staging tree
================================

http://www.masscrypt.io

Copyright (c) 2011-2014 Litecoin Developers
Copyright (c) 2017-2018 Masscoin Developers

What is Masscoin?
----------------

Masscoin is a lite version of Litecoin using scrypt as a proof-of-work algorithm.
 - 15 sec block targets
 - subsidy halves in 4204800 blocks (~2 years)
 - 270,000,000 total coins

 - Reward is 32 coins per block
 - 5760 blocks to retarget difficulty

For more information, as well as an immediately useable, binary version of
the Masscoin client sofware, see http://www.masscrypt.io

License
-------

Masscoin is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.

Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the Masscoin
development team members simply pulls it.

If it is a *more complicated or potentially controversial* change, then the patch
submitter will be asked to start a discussion with the devs and community.

The patch will be accepted if there is broad consensus that it is a good thing.
Developers should expect to rework and resubmit patches if the code doesn't
match the project's coding conventions (see `doc/coding.txt`) or are
controversial.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/auxesisgroup/masscoin/tags) are created
regularly to indicate new official, stable release versions of Masscoin.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write unit tests for new code, and to
submit new unit tests for old code.

Unit tests for the core code are in `src/test/`. To compile and run them:

    cd src; make -f makefile.unix test

Unit tests for the GUI code are in `src/qt/test/`. To compile and run them:

    qmake BITCOIN_QT_TEST=1 -o Makefile.test bitcoin-qt.pro
    make -f Makefile.test
    ./masscoin-qt_test

### Installeing masscoin in ubuntu system
  
  1)Download masscoind (executable) from https://github.com/auxesisgroup/masscoid/
  2)Through terminal go to source destination and type
     ``` ./masscoind -daemon```
     (Your will get a error message)
  3)Now go to root and type following command
     ```cd .masscoin```
    ``` vim masscoin.conf```
   ```  rpcuser=username ```
```     rpcpassword= password```
     
   4)Now repeat step 2 to get daemon active



### Installeing masscoin in windows system     
    
1)Download masscoind (executable) from https://github.com/auxesisgroup/masscoin.git/
2)Install  the instance through setup file inside out folder
And your masscoin instance runs in your system after sucessful installation

### Minning masscoin in using GPU

1)Download CGminer(preferable v3.7.2)
2)Goto source file using terminal and type following command

```./cgminer --scrypt -o http://127.0.0.1:8332 -u username -p paswrd --intensity d```

