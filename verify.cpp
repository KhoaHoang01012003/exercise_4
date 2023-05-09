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
    // Load public key from file
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    try {
        FileSource pub_key( "public.ec.key", true /*binary*/ );
        publicKey.Load( pub_key );
    } catch (const Exception& e) {
        std::cerr << "Error loading public key: " << e.what() << std::endl;
        return 1;
    }

    // Load message from file
    std::ifstream messageFile("message.txt");
    if (!messageFile) {
        std::cerr << "Error: Can't open message.txt." << std::endl;
        return 1;
    }
    std::string message((std::istreambuf_iterator<char>(messageFile)),
                         std::istreambuf_iterator<char>());

    // Load signature from file
    std::ifstream sigFile("signature.txt", std::ios::binary);
    if (!sigFile) {
        std::cerr << "Error: Can't open signature.txt." << std::endl;
        return 1;
    }
    std::string signature((std::istreambuf_iterator<char>(sigFile)),
                           std::istreambuf_iterator<char>());

    // Verify signature
    ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
    

    bool result = false;

    StringSource ss(signature+message, true /*pump all*/,
    new SignatureVerificationFilter(
        verifier,
        new ArraySink( (byte*)&result, sizeof(result) )
    ) // SignatureVerificationFilter
);
    if (result) {
        std::cout << "Signature verification succeeded." << std::endl;
    } else {
        std::cout << "Signature verification failed." << std::endl;
    }  
    std::cout << "message: " << message ;
    return 0;
}
