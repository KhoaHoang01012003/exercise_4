#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/sha.h>


using namespace CryptoPP;
using namespace CryptoPP::ASN1;

int main(int argc, char* argv[]) {
    AutoSeededRandomPool prng;

    // Đường cong elliptic
    DL_GroupParameters_EC<ECP> params(secp256r1());

    // Tạo private key
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    privateKey.Initialize(prng, params);

    // Tạo public key từ private key
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    // Save private key in PKCS #8 format
    FileSink pri_key( "private.ec.key", true /*binary*/ );
    privateKey.Save( pri_key );

    // Save public key in X.509 format
    FileSink pub_key( "public.ec.key", true /*binary*/ );
    publicKey.Save( pub_key );

     // Load message from file
    std::ifstream messageFile("message.txt");
    if (!messageFile) {
        std::cerr << "Error: Can't open message.txt." << std::endl;
        return 1;
    }
    std::string message((std::istreambuf_iterator<char>(messageFile)),
                         std::istreambuf_iterator<char>());

    // Tạo chữ ký
    ECDSA<ECP, SHA256>::Signer signer(privateKey);
    std::string signature;
    StringSource ss(message, true, 
        new SignerFilter(prng, signer,
            new StringSink(signature)
        )
    );
    // Lưu chữ ký vào file
    std::ofstream sigFile("signature.txt", std::ios::binary);
    if (!sigFile) {
        std::cerr << "Error: Can't open signature.txt." << std::endl;
        return 1;
    }
    sigFile.write(signature.data(), signature.size());
    sigFile.close();
    std::cout << std::endl << "Message signed successfully." << std::endl;

   privateKey.MakePublicKey( publicKey );

    bool result_PLK = publicKey.Validate( prng, 3 );
    if(result_PLK) {
        std::cout << "Public key hop le!";
    }
    else std::cout << "Public key KHONG hop le!";
    bool result_PRK = publicKey.Validate( prng, 3 );
    if(result_PRK) {
        std::cout << "Private key hop le!" << std::endl;
    }
    else std::cout << "Private key KHONG hop le!" << std::endl;
    std::cout << "message: " << message ;
    return 0;
}