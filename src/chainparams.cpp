// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021 The Raven Core developers
// Copyright (c) 2023-2024 The Aidp Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	CMutableTransaction txNew;
	txNew.nVersion = 1;
	txNew.vin.resize(1);
	txNew.vout.resize(1);
	txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
	txNew.vout[0].nValue = genesisReward;
	txNew.vout[0].scriptPubKey = genesisOutputScript;

	CBlock genesis;
	genesis.nTime	= nTime;
	genesis.nBits	= nBits;
	genesis.nNonce   = nNonce;
	genesis.nVersion = nVersion;
	genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
	genesis.hashPrevBlock.SetNull();
	genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
	return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	const char* pszTimestamp = "The FT 2024-04-18 Andreessen Horowitz raises 7.2bn and sets sights on AI start-ups";
	const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf22d5f") << OP_CHECKSIG;
	return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
	consensus.vDeployments[d].nStartTime = nStartTime;
	consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *	timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
	CMainParams() {
    	strNetworkID = "main";
    	consensus.nSubsidyHalvingInterval = 129600;  // 90 days at 1 min block time
    	consensus.nBIP34Enabled = true;
    	consensus.nBIP65Enabled = true;
    	consensus.nBIP66Enabled = true;
    	consensus.nSegwitEnabled = true;
    	consensus.nCSVEnabled = true;
    	consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    	consensus.kawpowLimit = uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
    	consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
    	consensus.nPowTargetSpacing = 1 * 60;
	consensus.fPowAllowMinDifficultyBlocks = false;
    	consensus.fPowNoRetargeting = false;
    	consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
    	consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1735603200; // 2024-12-31 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1814;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1735603200; // 2024-12-31 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1735603200; // 2024-12-31 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; 
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1735603200; // 2024-12-31 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; 
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1735603200; // 2024-12-31 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; 
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1735603200; // 2024-12-31 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; 
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;

   		uint32_t nGenesisTime = 1713481200; // 2024-04-18 23:00:00



    	// The best chain should have at least this much work
    	consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // New blockchain

    	// By default assume that the signatures in ancestors of this block are valid. Block# 0
    	consensus.defaultAssumeValid = uint256S("0x000000608970d8abc72d549cb0071ed7702a08d2149f71ddbe87ae2a16e5bc7c"); // Genesis block


    	pchMessageStart[0] = 0x41; // A
    	pchMessageStart[1] = 0x49; // I
    	pchMessageStart[2] = 0x44; // D
    	pchMessageStart[3] = 0x50; // P
    	nDefaultPort = 17001;
    	nPruneAfterHeight = 100000;

    	genesis = CreateGenesisBlock(nGenesisTime, 21677642, 0x1e00ffff, 4, 600 * COIN);

    	consensus.hashGenesisBlock = genesis.GetX16RHash();

    	printf("genesis.GetX16RHash = %s \n", genesis.GetX16RHash().ToString().c_str());
    	printf("Merkle = %s \n", genesis.hashMerkleRoot.ToString().c_str());
    	printf("Nonce = %u \n",  genesis.nNonce);

    	assert(consensus.hashGenesisBlock == uint256S("000000608970d8abc72d549cb0071ed7702a08d2149f71ddbe87ae2a16e5bc7c"));
    	assert(genesis.hashMerkleRoot == uint256S("e5730bfde1b85e96d78d415c5135cf3c789c37d895d8cbacf7e6e9d64c3f00af"));

    	vSeeds.emplace_back("dns.ai-depin.org", false);
    	vSeeds.emplace_back("eur-seed1.ai-depin.org", false);
    	vSeeds.emplace_back("eur-seed2.ai-depin.org", false);

    	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);  // X and 0x4c
    	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);  // X and 0x4c
    	base58Prefixes[SECRET_KEY] = 	std::vector<unsigned char>(1,128);
    	base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
    	base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

    	// Raven BIP44 cointype in mainnet is '0'
    	nExtCoinType = 0;

    	vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

    	fDefaultConsistencyChecks = false;
    	fRequireStandard = true;
    	fMineBlocksOnDemand = false;
    	fMiningRequiresPeers = true;

    	checkpointData = (CCheckpointData) {
        	{
    	{ 0, genesis.GetHash()}
        	}
    	};

    	chainTxData = ChainTxData{
        	nGenesisTime, // * UNIX timestamp of last known number of transactions
        	0,	// * total number of transactions between genesis and that timestamp
                    	//   (the tx=... number in the SetBestChain debug.log lines)
        	0   	// * estimated number of transactions per second after that timestamp
    	};

    	/** AIDP Start **/
    	// Burn Amounts
    	nIssueAssetBurnAmount = 50 * COIN;
    	nReissueAssetBurnAmount = 10 * COIN;
    	nIssueSubAssetBurnAmount = 10 * COIN;
    	nIssueUniqueAssetBurnAmount = 0.5 * COIN;
    	nIssueMsgChannelAssetBurnAmount = 10 * COIN;
    	nIssueQualifierAssetBurnAmount = 100 * COIN;
    	nIssueSubQualifierAssetBurnAmount = 10 * COIN;
    	nIssueRestrictedAssetBurnAmount = 150 * COIN;
    	nAddNullQualifierTagBurnAmount = 0.01 * COIN;

		// 10% of AIDP Rewards for AIDP investment
		nCommunityAutonomousAmount = 10;

    	// Burn Addresses
    	strIssueAssetBurnAddress = "XIissueAssetXXXXXXXXXXXXXXXXXhhZGt";
    	strReissueAssetBurnAddress = "XIReissueAssetXXXXXXXXXXXXXXVEFAWu";
    	strIssueSubAssetBurnAddress = "XIissueSubAssetXXXXXXXXXXXXXWcwhwL";
    	strIssueUniqueAssetBurnAddress = "XIissueUniqueAssetXXXXXXXXXXWEAe58";
    	strIssueMsgChannelAssetBurnAddress = "XIissueMsgChanneLAssetXXXXXXSjHvAY";
    	strIssueQualifierAssetBurnAddress = "XIissueQuaLifierXXXXXXXXXXXXUgEDbC";
    	strIssueSubQualifierAssetBurnAddress = "XIissueSubQuaLifierXXXXXXXXXVTzvv5";
    	strIssueRestrictedAssetBurnAddress = "XIissueRestrictedXXXXXXXXXXXXzJZ1q";
    	strAddNullQualifierTagBurnAddress = "XIaddTagBurnXXXXXXXXXXXXXXXXZQm5ya";

        //Global Burn Address
    	strGlobalBurnAddress = "XIBurnXXXXXXXXXXXXXXXXXXXXXXWUo9FV";

		// AIDP Fund
    	strCommunityAutonomousAddress = "XiVX62PZFGRqK9yo8qYFYBFNFCmLwkN7oi";
    	// DGW Activation
    	nDGWActivationBlock = 1;

    	nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
    	nMinReorganizationPeers = 4;
    	nMinReorganizationAge = 60 * 60 * 12; // 12 hours

    	nAssetActivationHeight = 1; // Asset activated block height
    	nMessagingActivationBlock = 1; // Messaging activated block height
    	nRestrictedActivationBlock = 1; // Restricted activated block height

    	nKAAAWWWPOWActivationTime = nGenesisTime + 1;
    	nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
    	/** AIDP End **/
	}
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams {
public:
	CTestNetParams() {
    	strNetworkID = "test";
    	consensus.nSubsidyHalvingInterval = 129600;  // 90 days at 1 min block time
    	consensus.nBIP34Enabled = true;
    	consensus.nBIP65Enabled = true;
    	consensus.nBIP66Enabled = true;
    	consensus.nSegwitEnabled = true;
    	consensus.nCSVEnabled = true;
    	consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    	consensus.kawpowLimit = uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
    	consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
    	consensus.nPowTargetSpacing = 1 * 60;
    	consensus.fPowAllowMinDifficultyBlocks = false;
    	consensus.fPowNoRetargeting = false;
    	consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
    	consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1711929600; // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1893452400;  //  2029-12-31 23:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1711929600;  // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1893452400;  //  2029-12-31 23:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1711929600;  // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1893452400; //  2029-12-31 23:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1711929600;  // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1893452400;  //  2029-12-31 23:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1711929600;  // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1893452400;  //  2029-12-31 23:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1711929600;  // 2024-04-01 00:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1893452400;  //  2029-12-31 23:00:00
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

    	// The best chain should have at least this much work.
    	consensus.nMinimumChainWork = uint256S("0x00");

    	// By default assume that the signatures in ancestors of this block are valid.
    	consensus.defaultAssumeValid = uint256S("0x00");


    	pchMessageStart[0] = 0x41; // A
    	pchMessageStart[1] = 0x49; // I
    	pchMessageStart[2] = 0x44; // D
    	pchMessageStart[3] = 0x50; // P	 
    	nDefaultPort = 17002;
    	nPruneAfterHeight = 1000;

    	uint32_t nGenesisTime = 1713481200; // 2024-04-18 23:00:00

  	 
    	genesis = CreateGenesisBlock(nGenesisTime, 21677642, 0x1e00ffff, 4, 600 * COIN);
    	consensus.hashGenesisBlock = genesis.GetX16RHash();


    	//Test MerkleRoot and GenesisBlock
    	assert(consensus.hashGenesisBlock == uint256S("0x000000608970d8abc72d549cb0071ed7702a08d2149f71ddbe87ae2a16e5bc7c"));
    	assert(genesis.hashMerkleRoot == uint256S("e5730bfde1b85e96d78d415c5135cf3c789c37d895d8cbacf7e6e9d64c3f00af"));

    	vFixedSeeds.clear();
    	vSeeds.clear();

    	vSeeds.emplace_back("dns.ai-depin.org", false);
    	vSeeds.emplace_back("eur-seed1.ai-depin.org", false);
    	vSeeds.emplace_back("eur-seed2.ai-depin.org", false);

    	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);  // X and 0x4c
    	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);  // X and 0x4c
    	base58Prefixes[SECRET_KEY] = 	std::vector<unsigned char>(1,239);
    	base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
    	base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

    	// Aidp BIP44 cointype in testnet
    	nExtCoinType = 1;

    	vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

    	fDefaultConsistencyChecks = false;
    	fRequireStandard = false;
    	fMineBlocksOnDemand = false;
    	fMiningRequiresPeers = true;

    	checkpointData = (CCheckpointData) {
        	{
                	{0, genesis.GetHash()}
        	}
    	};

    	chainTxData = ChainTxData{
        	// Update as we know more about the contents of the Raven chain
        	// Stats as of 00000023b66f46d74890287a7b1157dd780c7c5fdda2b561eb96684d2b39d62e window size 43200
        	0, // * UNIX timestamp of last known number of transactions
        	0, 	// * total number of transactions between genesis and that timestamp
                    	//   (the tx=... number in the SetBestChain debug.log lines)
        	0.0    	// * estimated number of transactions per second after that timestamp
    	};

    	/** AIDP Start **/
    	// Burn Amounts
    	nIssueAssetBurnAmount = 50 * COIN;
    	nReissueAssetBurnAmount = 10 * COIN;
    	nIssueSubAssetBurnAmount = 10 * COIN;
    	nIssueUniqueAssetBurnAmount = 0.5 * COIN;
    	nIssueMsgChannelAssetBurnAmount = 10 * COIN;
    	nIssueQualifierAssetBurnAmount = 100 * COIN;
    	nIssueSubQualifierAssetBurnAmount = 10 * COIN;
    	nIssueRestrictedAssetBurnAmount = 150 * COIN;
    	nAddNullQualifierTagBurnAmount = 0.01 * COIN;

	// 10% of AIDP Rewards for AIDP investment
	nCommunityAutonomousAmount = 10;

    	// Burn Addresses
    	strIssueAssetBurnAddress = "X1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
    	strReissueAssetBurnAddress = "X1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
    	strIssueSubAssetBurnAddress = "X1issueSubAssetXXXXXXXXXXXXXbNiH6v";
    	strIssueUniqueAssetBurnAddress = "X1issueUniqueAssetXXXXXXXXXXS4695i";
    	strIssueMsgChannelAssetBurnAddress = "X1issueMsgChanneLAssetXXXXXXT2PBdD";
    	strIssueQualifierAssetBurnAddress = "X1issueQuaLifierXXXXXXXXXXXXUysLTj";
    	strIssueSubQualifierAssetBurnAddress = "X1issueSubQuaLifierXXXXXXXXXYffPLh";
    	strIssueRestrictedAssetBurnAddress = "X1issueRestrictedXXXXXXXXXXXXZVT9V";
    	strAddNullQualifierTagBurnAddress = "X1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

	// Donation Address
    	strCommunityAutonomousAddress = "XiVX62PZFGRqK9yo8qYFYBFNFCmLwkN7oi";

    	// Global Burn Address
    	strGlobalBurnAddress = "X1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

    	// DGW Activation
    	nDGWActivationBlock = 1;

    	nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
    	nMinReorganizationPeers = 4;
    	nMinReorganizationAge = 60 * 60 * 12; // 12 hours

    	nAssetActivationHeight = 1; // Asset activated block height
    	nMessagingActivationBlock = 1; // Messaging activated block height
    	nRestrictedActivationBlock = 1; // Restricted activated block height

    	nKAAAWWWPOWActivationTime = nGenesisTime + 1;
    	nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
    	/** AIDP End **/
	}
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
	CRegTestParams() {
    	strNetworkID = "regtest";
    	consensus.nBIP34Enabled = true;
    	consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
    	consensus.nBIP66Enabled = true;
    	consensus.nSegwitEnabled = true;
    	consensus.nCSVEnabled = true;
    	consensus.nSubsidyHalvingInterval = 150;
    	consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    	consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    	consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
    	consensus.nPowTargetSpacing = 1 * 60;
    	consensus.fPowAllowMinDifficultyBlocks = true;
    	consensus.fPowNoRetargeting = true;
    	consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
    	consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1893452400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1893452400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0; // GMT: Sun Mar 3, 2019 5:00:00 PM
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1893452400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1893452400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
    	consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1893452400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
    	consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1893452400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
    	consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

    	// The best chain should have at least this much work.
    	consensus.nMinimumChainWork = uint256S("0x00");

    	// By default assume that the signatures in ancestors of this block are valid.
    	consensus.defaultAssumeValid = uint256S("0x00");

    	pchMessageStart[0] = 0x41; // A
    	pchMessageStart[1] = 0x49; // I
    	pchMessageStart[2] = 0x44; // D
    	pchMessageStart[3] = 0x50; // P
    	nDefaultPort = 17003;
    	nPruneAfterHeight = 1000;

   	    uint32_t nGenesisTime = 1713481200; // 2024-04-18 23:00:00


    	genesis = CreateGenesisBlock(nGenesisTime, 1, 0x207fffff, 4, 600 * COIN);
    	consensus.hashGenesisBlock = genesis.GetX16RHash();

    	assert(consensus.hashGenesisBlock == uint256S("0000005c0ce06920448c82e49182eccf3dd0e360e317ddbbca1adf69cb5ab171 "));
    	assert(genesis.hashMerkleRoot == uint256S("61fe2a3916485577139a8fd9422aeab891d33f7cd6cf36ba8c67841cb4f8e0cd"));

    	vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
    	vSeeds.clear();  	//!< Regtest mode doesn't have any DNS seeds.

    	fDefaultConsistencyChecks = true;
    	fRequireStandard = false;
    	fMineBlocksOnDemand = true;

    	checkpointData = (CCheckpointData) {
        	{
        	}
    	};

    	chainTxData = ChainTxData{
        	0,
        	0,
        	0
    	};

    	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);  // X and 0x4c
    	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);  // X and 0x4c
    	base58Prefixes[SECRET_KEY] = 	std::vector<unsigned char>(1,239);
    	base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
    	base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

    	// Raven BIP44 cointype in regtest
    	nExtCoinType = 1;

    	/** AIDP Start **/
    	// Burn Amounts
    	nIssueAssetBurnAmount = 50 * COIN;
    	nReissueAssetBurnAmount = 10 * COIN;
    	nIssueSubAssetBurnAmount = 10 * COIN;
    	nIssueUniqueAssetBurnAmount = 0.5 * COIN;
    	nIssueMsgChannelAssetBurnAmount = 10 * COIN;
    	nIssueQualifierAssetBurnAmount = 100 * COIN;
    	nIssueSubQualifierAssetBurnAmount = 10 * COIN;
    	nIssueRestrictedAssetBurnAmount = 150 * COIN;
    	nAddNullQualifierTagBurnAmount = 0.01 * COIN;

	// 10% of AIDP Rewards for AIDP investment
	nCommunityAutonomousAmount = 10;

    	// Burn Addresses
    	strIssueAssetBurnAddress = "X1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
    	strReissueAssetBurnAddress = "X1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
    	strIssueSubAssetBurnAddress = "X1issueSubAssetXXXXXXXXXXXXXbNiH6v";
    	strIssueUniqueAssetBurnAddress = "X1issueUniqueAssetXXXXXXXXXXS4695i";
    	strIssueMsgChannelAssetBurnAddress = "X1issueMsgChanneLAssetXXXXXXT2PBdD";
    	strIssueQualifierAssetBurnAddress = "X1issueQuaLifierXXXXXXXXXXXXUysLTj";
    	strIssueSubQualifierAssetBurnAddress = "X1issueSubQuaLifierXXXXXXXXXYffPLh";
    	strIssueRestrictedAssetBurnAddress = "X1issueRestrictedXXXXXXXXXXXXZVT9V";
    	strAddNullQualifierTagBurnAddress = "X1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

	// Donation Address
    	strCommunityAutonomousAddress = "XqC44qxWUKbhd7DMqEGESVLRDPzLMZJWXA";

    	// Global Burn Address
    	strGlobalBurnAddress = "X1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

    	// DGW Activation
    	nDGWActivationBlock = 200;

    	nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
    	nMinReorganizationPeers = 4;
    	nMinReorganizationAge = 60 * 60 * 12; // 12 hours

    	nAssetActivationHeight = 0; // Asset activated block height
    	nMessagingActivationBlock = 0; // Messaging activated block height
    	nRestrictedActivationBlock = 0; // Restricted activated block height

    	// TODO, we need to figure out what to do with this for regtest. This effects the unit tests
    	// For now we can use a timestamp very far away
    	// If you are looking to test the kawpow hashing function in regtest. You will need to change this number
    	nKAAAWWWPOWActivationTime = 3582830167;
    	nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
    	/** AIDP End **/
	}
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &GetParams() {
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

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
	SelectBaseParams(network);
	if (fForceBlockNetwork) {
    	bNetwork.SetNetwork(network);
	}
	globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
	globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
