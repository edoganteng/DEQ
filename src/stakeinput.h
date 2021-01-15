// Copyright (c) 2017-2018 The PIVX developers
// Copyright (c) 2018-2021 The Dequant developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef Dequant_STAKEINPUT_H
#define Dequant_STAKEINPUT_H

class CKeyStore;
class CWallet;
class CWalletTx;

class CStakeInput
{
protected:
    CBlockIndex* pindexFrom;

public:
    virtual ~CStakeInput(){};
    virtual CBlockIndex* GetIndexFrom() = 0;
    virtual bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = 0) = 0;
    virtual bool GetTxFrom(CTransaction& tx) = 0;
    virtual CAmount GetValue() = 0;
    virtual bool CreateTxOuts(CWallet* pwallet, vector<CTxOut>& vout, CAmount nTotal) = 0;
    virtual bool GetModifier(uint64_t& nStakeModifier) = 0;
    virtual bool IsZDEQ() = 0;
    virtual CDataStream GetUniqueness() = 0;
};


// zdeqStake can take two forms
// 1) the stake candidate, which is a zcmint that is attempted to be staked
// 2) a staked zdeq, which is a zcspend that has successfully staked
class CZDEQStake : public CStakeInput
{
private:
    uint32_t nChecksum;
    bool fMint;
    libzerocoin::CoinDenomination denom;
    uint256 hashSerial;

public:
    explicit CZDEQStake(libzerocoin::CoinDenomination denom, const uint256& hashSerial)
    {
        this->denom = denom;
        this->hashSerial = hashSerial;
        this->pindexFrom = nullptr;
        fMint = true;
    }

    explicit CZDEQStake(const libzerocoin::CoinSpend& spend);

    CBlockIndex* GetIndexFrom() override;
    bool GetTxFrom(CTransaction& tx) override;
    CAmount GetValue() override;
    bool GetModifier(uint64_t& nStakeModifier) override;
    CDataStream GetUniqueness() override;
    bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = 0) override;
    bool CreateTxOuts(CWallet* pwallet, vector<CTxOut>& vout, CAmount nTotal) override;
    bool MarkSpent(CWallet* pwallet, const uint256& txid);
    bool IsZDEQ() override { return true; }
    int GetChecksumHeightFromMint();
    int GetChecksumHeightFromSpend();
    uint32_t GetChecksum();
};

class CDEQStake : public CStakeInput
{
private:
    CTransaction txFrom;
    unsigned int nPosition;
public:
    CDEQStake()
    {
        this->pindexFrom = nullptr;
    }

    bool SetInput(CTransaction txPrev, unsigned int n);

    CBlockIndex* GetIndexFrom() override;
    bool GetTxFrom(CTransaction& tx) override;
    CAmount GetValue() override;
    bool GetModifier(uint64_t& nStakeModifier) override;
    CDataStream GetUniqueness() override;
    bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = 0) override;
    bool CreateTxOuts(CWallet* pwallet, vector<CTxOut>& vout, CAmount nTotal) override;
    bool IsZDEQ() override { return false; }
};


#endif //Dequant_STAKEINPUT_H
