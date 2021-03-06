using namespace TpmCpp;


#ifndef __linux__

#ifdef ATTESTATIONLIB_EXPORTS
#define ATTESTATIONLIB_API __declspec(dllexport)
#else
#define ATTESTATIONLIB_API __declspec(dllimport)
#endif
#else

#define ATTESTATIONLIB_API

#endif

// This class is exported from the AttestationLib.dll
class ATTESTATIONLIB_API CAttestationLib {
public:
	CAttestationLib(void);
    ~CAttestationLib(void);
    void Initialize(std::wstring attestationServerHost, std::wstring attestationServerScheme);
    bool CreateAttestationIdentityKey();
    bool CreateSealedUserKey();
    bool SaveSealedUserKey(ByteVec &serializedKey);
    bool LoadSealedUserKey(ByteVec &serializedKey);
    bool CheckUserKeyWhitelist();
    void ShowTpmCapabilities();
    bool SignAndVerifyMessage(const std::string &message);
    bool SignHash(const ByteVec &hashBytes, ByteVec &signatureBytes);
    bool VerifySignature(const ByteVec &hashBytes, const ByteVec &signatureBytes);
    ByteVec GetEkPubHashBytes();
    std::string GetUserPubHashHex();
    ByteVec GetUserPubModulus();

private:
    void SetPlatformAuthenticationValues();
    TPM_HANDLE MakeChildSigningKey(TPM_HANDLE parentHandle, bool restricted);
    TPM_HANDLE MakeStoragePrimary();
    TPM_HANDLE MakeEndorsementKey();
    bool RestLookupRegisteredKey(TPMT_PUBLIC &clientPub);
    bool RestRegisterKey(
        TPMT_PUBLIC &clientRestrictedPub,
        PCR_ReadResponse &clientPcrVals,
        QuoteResponse &clientPcrQuote,
        TPMS_CREATION_DATA &clientKeyCreation,
        CertifyCreationResponse &clientKeyQuote);
    bool RestGetActivation(
        TPMT_PUBLIC &clientEkPub,
        TPMT_PUBLIC &clientRestrictedPub,
        ActivationData &activationData);

private:
    utility::string_t m_attestationServerHost;
    utility::string_t m_attestationServerScheme;
    TpmDevice *m_pDevice;
    Tpm2 m_tpm;
    CreatePrimaryResponse m_ekCreate;
    TPM_HANDLE m_hEk;
    TPMT_PUBLIC m_ekPub;
    //TPM_HANDLE m_hSrk;
    //TPMT_PUBLIC m_srkPub;
    CreateResponse m_aikCreate;
    TPM_HANDLE m_hAik;
    TPMT_PUBLIC m_aikPub;
    CreateResponse m_userCreate;
    TPM_HANDLE m_hUser;
    TPMT_PUBLIC m_userPub;
    ByteVec m_decryptedTpmSecret;
};
