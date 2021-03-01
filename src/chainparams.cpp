// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2019-2020 The MasterWin developers
// Copyright (c) 2018-2021 The Dequant developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[0];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x0000016644fd343c4a49f5479d35a634a7a599236fe8d2223c5b8679cac3bad7"))
	(275, uint256("0x59d4c0b191da8df077e31f349d36506fce57f4bcd3cf7fd8a0732d2fa7783953"))
	(4480, uint256("0xf8648e78e0ddd8358d8371c0b9bfabf18c15c78519f6a1368735bdfab41f8675"))
	(5433, uint256("0x49fa1734806a25b76b7878b16172bc9a017bafe9ec8355d29af45a6b37279532"))
	(12000, uint256("0x72335b0212372b16b86f7e3bf302d3c09423f95d77112494b91ae41241b85655"))
	(22712, uint256("0xd3fc4e06c60919ef90445dd40f10aef311a6ba4346693e32c3079dde8c2a39fe"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1614590351, // * UNIX timestamp of last checkpoint block
    50310,		// * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1608423380,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1608423382,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xb2;
        pchMessageStart[1] = 0x5e;
        pchMessageStart[2] = 0xca;
        pchMessageStart[3] = 0x1d;
		vAlertPubKey = ParseHex("04aa935558eb828330afa24913d88b5ea8e00601091a890a1f09627b1e75338268e881f610308df6f730410f300b488a4742cb8102ca6e16b413a11220947abd7a");
		nDefaultPort = 32820;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Dequant starting difficulty is 1 / 2^12
        //nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // DEQ: 1 day
        nTargetSpacing = 2 * 60;  // DEQ: 2 minute
        nMaturity = 61;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 50000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 100;
        //nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 1608423383;
        nZerocoinStartTime = 1608423383;
        nBlockEnforceSerialRange = 253; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 9999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nEnforceNewSporkKey = 1574428163; //!> Sporks signed after (GMT):  Wednesday, June 12, 2019 5:00:00 AM GMT must use the new spork key

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Dequant Genesis Block: 2018/12/28 12:30:00 Created by developer.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		txNew.vout[0].SetEmpty();
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04f594cc2cfabe09aa8a7545d21819be287a0713492a3a61f52917d797c119b868c64f350fc93cd731e38b4d1b1174155cc0b63abbd86deea395574d568721913d") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1608423383;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 8435017;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000016644fd343c4a49f5479d35a634a7a599236fe8d2223c5b8679cac3bad7"));
        assert(genesis.hashMerkleRoot == uint256("0x5ef2d9c4a225776d4f6e65f59833f67d3c057bbcd9095e96c798e8e74df234bb"));

        vSeeds.push_back(CDNSSeedData("1", "88.198.95.105"));
        vSeeds.push_back(CDNSSeedData("2", "95.217.213.61"));
		vSeeds.push_back(CDNSSeedData("3", "195.22.153.54"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);     //starts with 'D'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 31);    //starts with 'D'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 242);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x22)(0x2e)(0x35)(0x13).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x52)(0xf1)(0xe1)(0x1B).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x60)(0x70)(0x80)(0x97).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "041ecbc5ac6d4aacd43f3d635ed306017b04b2a9b93c57ae3a02e6675529026f231857ddc2b3a2c77c5cb422b35869f00a7c293deca4454851761436457dc2d527";
        strObfuscationPoolDummyAddress = "ZAqzopELgLz7zxaAZVrT74tShsDuDjaMsz";
        nStartMasternodePayments = 1608423365; 

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zdeq to be stakable
        
    vMasternodePhasen.push_back ({
      0,
      {
        {   50 * COIN, 90 }
      }
    });
    vMasternodePhasen.push_back ({
      17000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 90 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      30000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 90 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      50000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 90 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      80000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 90 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      100000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 90 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      125000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 90 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      150000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 90 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      200000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 90 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      250000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 90 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      300000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 90 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      400000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 90 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      500000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 90 },
        { 150000 * COIN, 0 }
      }
    });
	  vMasternodePhasen.push_back ({
      750000,
      {
        {  	  50 * COIN, 0 },
        {    250 * COIN, 0 },
        {  	 500 * COIN, 0 },
        {  	1000 * COIN, 0 },
        {  	2500 * COIN, 0 },
        {  	5000 * COIN, 0 },
        {  	7500 * COIN, 0 },
        {  10000 * COIN, 0 },
        {  20000 * COIN, 0 },
        {  30000 * COIN, 0 },
        {  40000 * COIN, 0 },
        {  80000 * COIN, 0 },
        { 100000 * COIN, 0 },
        { 150000 * COIN, 90 }
      }
    });
  }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x43;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0xba;
		vAlertPubKey = ParseHex("0469cddd2b8662f65477e729d283610f448359344a8aa1ce824e279c1a5db24de59e7cc8544c14185241d4ab048c6e70146f084a71bc32285edec7c83ddbb2e868");
		nDefaultPort = 32822;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Dequant: 1 day
        nTargetSpacing = 1 * 60;  // Dequant: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1576238085;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("", ""));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 38); // Testnet mwx addresses start with 'g' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet mwx script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet mwx BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet mwx BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet mwx BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

		convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 2;
        strSporkKey = "04af40fe063f852980847986bb6f5041ecbc5ac6d4aacd43f3d635ed306017b04b2a9b93c57ae3a02e6675529026f231857ddc2b3a2c77c5cb422b35869f00a7c293deca4454851761436457dc2d527a491674a6e424dfb0d7d26b47ee65f43965525efad6b2b1b923894eb9408efdb410ae4c8d7b45b8dba749f511d3063392674b";
        strObfuscationPoolDummyAddress = "ZAqzopELgLz7zxaAZVrT74tShsDuDjaMsz";
        nStartMasternodePayments = 1576238087; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x69;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        //nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Dequant: 1 day
        nTargetSpacing = 1 * 60;        // Dequant: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1576238186;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 32824;
        //assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 32825;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    //virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
