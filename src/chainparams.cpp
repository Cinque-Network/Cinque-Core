// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The DigiByte Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "arith_uint256.h"
#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = 12.5;
    txNew.vout[0].scriptPubKey = CScript() << 0x0 << OP_CHECKSIG;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "London Bridge Is Down"; // for testing
    const CScript genesisOutputScript = CScript() << 0x0 << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        //consensus.nSubsidyHalvingInterval = 210000; - DGB
        consensus.BIP16Exception = uint256S("0x0");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00"); // activate upon launch
        consensus.BIP65Height = 0; //
        consensus.BIP66Height = 0; //

        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.initialTarget[ALGO_ODO] = ArithToUint256(~arith_uint256(0) >> 40); // 256 difficulty
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 60 / 4;

        /** Current DigiByte 2017 Difficulty Adjustment Code & Block Target. See explanation here: 
        https://github.com/digibyte/digibyte-old/pull/36 
        https://github.com/digibyte/digibyte-old/pull/15

        Difficulty is updated for every algorithm on every block, not just the algorithm that was solved. 
        In particular, the difficulty of one algorithm may decrease when a different algorithm is solved. 

        An attacker with 90% of the SHA256D hashrate and 33% of each of the other 4 algorithms would 
        have insufficient hashpower to mount a 51% attack.

        - MultiAlgo POW (Scrypt, SHA256D, Qubit, Skein and Groestl) algorithms
        - 15 Second Block Target (1.5 min per Algo)
        - ~21 billion total coins in 21 years
        - 8000 coins per block, reduces by 0.5% every 10,080 blocks starting 2/28/14 1% monthly reduction
        - Difficulty retarget every 1 block per algo (1.5 Min)
        **/

        consensus.nTargetTimespan =  0.10 * 24 * 60 * 60; // 2.4 hours
        consensus.nTargetSpacing = 60; // 60 seconds
        consensus.nInterval = consensus.nTargetTimespan / consensus.nTargetSpacing;
        consensus.nDiffChangeTarget = 67200; // DigiShield Hard Fork Block BIP34Height 67,200

        // Old 1% monthly DGB Reward before 15 secon block change
        consensus.patchBlockRewardDuration = 10080; //10080; - No longer used
        //4 blocks per min, x60 minutes x 24hours x 14 days = 80,160 blocks for 0.5% reduction in DGB reward supply - No longer used
        consensus.patchBlockRewardDuration2 = 80160; //80160;
        consensus.nTargetTimespanRe = 1*60; // 60 Seconds
        consensus.nTargetSpacingRe = 1*60; // 60 seconds
        consensus.nIntervalRe = consensus.nTargetTimespanRe / consensus.nTargetSpacingRe; // 1 block

        consensus.nAveragingInterval = 10; // 10 blocks
        consensus.multiAlgoTargetSpacing = 30*5; // NUM_ALGOS * 30 seconds
        consensus.multiAlgoTargetSpacingV4 = 15*5; // NUM_ALGOS * 15 seconds
        consensus.nAveragingTargetTimespan = consensus.nAveragingInterval * consensus.multiAlgoTargetSpacing; // 10* NUM_ALGOS * 30
        consensus.nAveragingTargetTimespanV4 = consensus.nAveragingInterval * consensus.multiAlgoTargetSpacingV4; // 10 * NUM_ALGOS * 15

        consensus.nMaxAdjustDown = 40; // 40% adjustment down
        consensus.nMaxAdjustUp = 20; // 20% adjustment up
        consensus.nMaxAdjustDownV3 = 16; // 16% adjustment down
        consensus.nMaxAdjustUpV3 = 8; // 8% adjustment up
        consensus.nMaxAdjustDownV4 = 16;
        consensus.nMaxAdjustUpV4 = 8;

        consensus.nMinActualTimespan = consensus.nAveragingTargetTimespan * (100 - consensus.nMaxAdjustUp) / 100;
        consensus.nMaxActualTimespan = consensus.nAveragingTargetTimespan * (100 + consensus.nMaxAdjustDown) / 100;
        consensus.nMinActualTimespanV3 = consensus.nAveragingTargetTimespan * (100 - consensus.nMaxAdjustUpV3) / 100;
        consensus.nMaxActualTimespanV3 = consensus.nAveragingTargetTimespan * (100 + consensus.nMaxAdjustDownV3) / 100;
        consensus.nMinActualTimespanV4 = consensus.nAveragingTargetTimespanV4 * (100 - consensus.nMaxAdjustUpV4) / 100;
        consensus.nMaxActualTimespanV4 = consensus.nAveragingTargetTimespanV4 * (100 + consensus.nMaxAdjustDownV4) / 100;

        consensus.nLocalTargetAdjustment = 4; //target adjustment per algo
        consensus.nLocalDifficultyAdjustment = 4; //difficulty adjustment per algo


        // DigiByte Hard Fork Block Heights
        consensus.multiAlgoDiffChangeTarget = 145000; // Block 145,000 MultiAlgo Hard Fork
        consensus.alwaysUpdateDiffChangeTarget = 400000; // Block 400,000 MultiShield Hard Fork
        consensus.workComputationChangeTarget = 1430000; // Block 1,430,000 DigiSpeed Hard Fork
        consensus.algoSwapChangeTarget = 9100000; // Block 9,100,000 Odo PoW Hard Fork

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 28224; // 28224 - 70% of 40320
        consensus.nMinerConfirmationWindow = 40320; // nPowTargetTimespan / nPowTargetSpacing 40320 main net - 1 week
        consensus.fRbfEnabled = false;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 27; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 12; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1489997089; // March 24th, 2017 1490355345
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1521891345; // March 24th, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 13; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1490355345; // March 24th, 2017 1490355345
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1521891345; // March 24th, 2018

        // Deployment of BIP65, BIP66, and BIP34.
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].bit = 14; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nStartTime = 1489997089; // March 24th, 2017 1490355345
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nTimeout = 1521891345;    // March 24th, 2018

        // Reservation of version bits for future algos
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 12;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = 1542672000; // 20 Nov, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = 1574208000;   // 20 Nov, 2019

        // Deployment of Odo proof-of-work hardfork
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].nStartTime = 1556668800; // 1 May, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].nTimeout = 1588291200;    // 1 May, 2020

        // Deployment of Equihash algo softfork
        //consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].bit = 3;
        //consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nStartTime = 1489997089; // July, 2017 
        //consensus.vDeployments[Consensus::DEPLOYMENT_EQUIHASH].nTimeout = 1521891345;    // July, 2018

        // Deployment of Ethash algo softfork
        //consensus.vDeployments[Consensus::DEPLOYMENT_ETHASH].bit = 4;
        //consensus.vDeployments[Consensus::DEPLOYMENT_ETHASH].nStartTime = 1489997089; // October, 2017 
        //consensus.vDeployments[Consensus::DEPLOYMENT_ETHASH].nTimeout = 1521891345;    // October, 2018

        consensus.nOdoShapechangeInterval = 10*24*60*60; // 10 days

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); // Block 0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 12024;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1648274400, 4080782383, 0x1d00ffff, 1, 12.5);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c"));
        assert(genesis.hashMerkleRoot == uint256S("0x50fa04e624640fde15b97765a8f2468b9495012275ce96f8834a3bf01b702ee1"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        //vSeeds.emplace_back("seed.digibyte.org"); // Website collective
        //vSeeds.emplace_back("seed.digibyteservers.io"); // ChillingSilence


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,30);
        base58Prefixes[SCRIPT_ADDRESS_OLD] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[SECRET_KEY_OLD] = std::vector<unsigned char>(1,158);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "cnq";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
         {
            {     0, uint256S("0x00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c")},
         }
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c (height 0).
            /* nTime    */ 1648274400,
            /* nTxCount */ 0,
            /* dTxRate  */ 0.0
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.initialTarget[ALGO_ODO] = ArithToUint256(~arith_uint256(0) >> 36); // 16 difficulty
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 60 / 4;

        /** Current DigiByte 2017 Difficulty Adjustment Code & Block Target. See explanation here: 
        https://github.com/digibyte/digibyte-old/pull/36 
        https://github.com/digibyte/digibyte-old/pull/15

        Difficulty is updated for every algorithm on every block, not just the algorithm that was solved. 
        In particular, the difficulty of one algorithm may decrease when a different algorithm is solved. 

        An attacker with 90% of the SHA256D hashrate and 33% of each of the other 4 algorithms would 
        have insufficient hashpower to mount a 51% attack.

        - MultiAlgo POW (Scrypt, SHA256D, Qubit, Skein and Groestl) algorithms
        - 15 Second Block Target (1.5 min per Algo)
        - ~21 billion total coins in 21 years
        - 8000 coins per block, reduces by 0.5% every 10,080 blocks starting 2/28/14 1% monthly reduction
        - Difficulty retarget every 1 block per algo (1.5 Min)
        **/

        consensus.nTargetTimespan =  0.10 * 24 * 60 * 60; // 2.4 hours
        consensus.nTargetSpacing = 60; // 60 seconds
        consensus.nInterval = consensus.nTargetTimespan / consensus.nTargetSpacing;
        consensus.nDiffChangeTarget = 67; // DigiShield Hard Fork Block BIP34Height 67,200

        // Old 1% monthly DGB Reward before 15 secon block change
        consensus.patchBlockRewardDuration = 10; //10080; - No longer used
        //4 blocks per min, x60 minutes x 24hours x 14 days = 80,160 blocks for 0.5% reduction in DGB reward supply - No longer used
        consensus.patchBlockRewardDuration2 = 80; //80160;
        consensus.nTargetTimespanRe = 1*60; // 60 Seconds
        consensus.nTargetSpacingRe = 1*60; // 60 seconds
        consensus.nIntervalRe = consensus.nTargetTimespanRe / consensus.nTargetSpacingRe; // 1 block

        consensus.nAveragingInterval = 10; // 10 blocks
        consensus.multiAlgoTargetSpacing = 30*5; // NUM_ALGOS * 30 seconds
        consensus.multiAlgoTargetSpacingV4 = 15*5; // NUM_ALGOS * 15 seconds
        consensus.nAveragingTargetTimespan = consensus.nAveragingInterval * consensus.multiAlgoTargetSpacing; // 10* NUM_ALGOS * 30
        consensus.nAveragingTargetTimespanV4 = consensus.nAveragingInterval * consensus.multiAlgoTargetSpacingV4; // 10 * NUM_ALGOS * 15

        consensus.nMaxAdjustDown = 40; // 40% adjustment down
        consensus.nMaxAdjustUp = 20; // 20% adjustment up
        consensus.nMaxAdjustDownV3 = 16; // 16% adjustment down
        consensus.nMaxAdjustUpV3 = 8; // 8% adjustment up
        consensus.nMaxAdjustDownV4 = 16;
        consensus.nMaxAdjustUpV4 = 8;

        consensus.nMinActualTimespan = consensus.nAveragingTargetTimespan * (100 - consensus.nMaxAdjustUp) / 100;
        consensus.nMaxActualTimespan = consensus.nAveragingTargetTimespan * (100 + consensus.nMaxAdjustDown) / 100;
        consensus.nMinActualTimespanV3 = consensus.nAveragingTargetTimespan * (100 - consensus.nMaxAdjustUpV3) / 100;
        consensus.nMaxActualTimespanV3 = consensus.nAveragingTargetTimespan * (100 + consensus.nMaxAdjustDownV3) / 100;
        consensus.nMinActualTimespanV4 = consensus.nAveragingTargetTimespanV4 * (100 - consensus.nMaxAdjustUpV4) / 100;
        consensus.nMaxActualTimespanV4 = consensus.nAveragingTargetTimespanV4 * (100 + consensus.nMaxAdjustDownV4) / 100;

        consensus.nLocalTargetAdjustment = 4; //target adjustment per algo
        consensus.nLocalDifficultyAdjustment = 4; //difficulty adjustment per algo


        // DigiByte Hard Fork Block Heights
        consensus.multiAlgoDiffChangeTarget = 100; // Block 145,000 MultiAlgo Hard Fork
        consensus.alwaysUpdateDiffChangeTarget = 400; // Block 400,000 MultiShield Hard Fork
        consensus.workComputationChangeTarget = 1430; // Block 1,430,000 DigiSpeed Hard Fork
        consensus.algoSwapChangeTarget = 20000; // Block 9,000,000 Odo PoW Hard Fork

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 4032; // 4032 - 70% of 5760
        consensus.nMinerConfirmationWindow = 5760; // 1 day of blocks on testnet
        consensus.fRbfEnabled = false;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 27; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 12; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 13; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP65, BIP66, and BIP34.
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].bit = 14; //Add VERSIONBITS_NUM_BITS_TO_SKIP (12)
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_NVERSIONBIPS].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Reservation of version bits for future algos
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 12;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of Odo proof-of-work hardfork
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].nStartTime = 1551398400; // 1 Mar, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.nOdoShapechangeInterval = 1*24*60*60; // 1 day

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //1079274

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xc8;
        pchMessageStart[2] = 0xbd;
        pchMessageStart[3] = 0xdd;
        nDefaultPort = 12026;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1648274400, 4080782383, 0x1d00ffff, 1, 12.5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c"));
        assert(genesis.hashMerkleRoot == uint256S("0x50fa04e624640fde15b97765a8f2468b9495012275ce96f8834a3bf01b702ee1"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        //vSeeds.emplace_back("seed.testnet-1.us.digibyteservers.io");
        //vSeeds.emplace_back("seed.testnetexplorer.digibyteservers.io");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,126);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,140);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,254);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "cnqt";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                {     0, uint256S("0x00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c")},
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c
            /* nTime    */ 1648274400,
            /* nTxCount */ 0,
            /* dTxRate  */ 0.0
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 60 / 4;
        consensus.nTargetTimespan =  0.10 * 24 * 60 * 60; // 2.4 hours
        consensus.nTargetSpacing = 60; // 60 seconds
        consensus.nInterval = consensus.nTargetTimespan / consensus.nTargetSpacing;
        consensus.nDiffChangeTarget = 167; // DigiShield Hard Fork Block BIP34Height 67,200

        // Old 1% monthly DGB Reward before 15 secon block change
        consensus.patchBlockRewardDuration = 10; //10080; - No longer used
        //4 blocks per min, x60 minutes x 24hours x 14 days = 80,160 blocks for 0.5% reduction in DGB reward supply - No longer used
        consensus.patchBlockRewardDuration2 = 80; //80;
        consensus.nTargetTimespanRe = 1*60; // 60 Seconds
        consensus.nTargetSpacingRe = 1*60; // 60 seconds
        consensus.nIntervalRe = consensus.nTargetTimespanRe / consensus.nTargetSpacingRe; // 1 block

        consensus.nAveragingInterval = 10; // 10 blocks
        consensus.multiAlgoTargetSpacing = 30*5; // NUM_ALGOS * 30 seconds
        consensus.multiAlgoTargetSpacingV4 = 15*5; // NUM_ALGOS * 15 seconds
        consensus.nAveragingTargetTimespan = consensus.nAveragingInterval * consensus.multiAlgoTargetSpacing; // 10* NUM_ALGOS * 30
        consensus.nAveragingTargetTimespanV4 = consensus.nAveragingInterval * consensus.multiAlgoTargetSpacingV4; // 10 * NUM_ALGOS * 15

        consensus.nMaxAdjustDown = 40; // 40% adjustment down
        consensus.nMaxAdjustUp = 20; // 20% adjustment up
        consensus.nMaxAdjustDownV3 = 16; // 16% adjustment down
        consensus.nMaxAdjustUpV3 = 8; // 8% adjustment up
        consensus.nMaxAdjustDownV4 = 16;
        consensus.nMaxAdjustUpV4 = 8;

        consensus.nMinActualTimespan = consensus.nAveragingTargetTimespan * (100 - consensus.nMaxAdjustUp) / 100;
        consensus.nMaxActualTimespan = consensus.nAveragingTargetTimespan * (100 + consensus.nMaxAdjustDown) / 100;
        consensus.nMinActualTimespanV3 = consensus.nAveragingTargetTimespan * (100 - consensus.nMaxAdjustUpV3) / 100;
        consensus.nMaxActualTimespanV3 = consensus.nAveragingTargetTimespan * (100 + consensus.nMaxAdjustDownV3) / 100;
        consensus.nMinActualTimespanV4 = consensus.nAveragingTargetTimespanV4 * (100 - consensus.nMaxAdjustUpV4) / 100;
        consensus.nMaxActualTimespanV4 = consensus.nAveragingTargetTimespanV4 * (100 + consensus.nMaxAdjustDownV4) / 100;

        consensus.nLocalTargetAdjustment = 4; //target adjustment per algo
        consensus.nLocalDifficultyAdjustment = 4; //difficulty adjustment per algo

        consensus.BIP65Height = 1351;
        consensus.BIP66Height = 1251;

        // DigiByte Hard Fork Block Heights
        consensus.multiAlgoDiffChangeTarget = 145; // Block 145,000 MultiAlgo Hard Fork
        consensus.alwaysUpdateDiffChangeTarget = 400; // Block 400,000 MultiShield Hard Fork
        consensus.workComputationChangeTarget = 1430; // Block 1,430,000 DigiSpeed Hard Fork
        consensus.algoSwapChangeTarget = 2000; // Block 9,000,000 Odo PoW Hard Fork
        consensus.nMinerConfirmationWindow = 3600; // 1 hour in RegTest

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_RESERVEALGO].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ODO].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;

        consensus.nOdoShapechangeInterval = 60; // 1 minute

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1648274400, 4080782383, 0x1d00ffff, 1, 12.5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c"));
        assert(genesis.hashMerkleRoot == uint256S("0x50fa04e624640fde15b97765a8f2468b9495012275ce96f8834a3bf01b702ee1"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("0x00000000bd4cd94c578b673e1404e92cfecc80fa51f612f5aa77e100a2ed7b1c")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,126);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,140);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,254);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "cnqrt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
