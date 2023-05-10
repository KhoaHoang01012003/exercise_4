//verify.cpp
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
    bool result;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; i++)
    {
        result = false;
        StringSource ss(signature+message, true,
                        new SignatureVerificationFilter(verifier, 
                            new ArraySink( (byte*)&result, sizeof(result) )
                            )
                        );
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    if (result) {
        std::cout << "Signature verification succeeded." << std::endl;
    } else {
        std::cout << "Signature verification failed." << std::endl;
    }  
    std::cout << "message: " << message ;

    std::cout << "\n\nTotal time for 100 rounds: " << duration.count() * 1.0 / 1000 << " ms" << std::endl;
    std::cout << "Average time for 100 rounds :" << (duration.count() * 1.0 / 1000) / 100 << " ms" << std::endl;
    return 0;
}