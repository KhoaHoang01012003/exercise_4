#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/oids.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>




using namespace CryptoPP;
using namespace CryptoPP::ASN1;

int main(int argc, char* argv[]) {
    // Đọc khóa công khai từ file
    ECDSA<ECP, SHA256>::PublicKey publicKey;

    FileSource publicKeyFile("public_key.ber", true);
    BERSequenceDecoder publicKeySeq(publicKeyFile);

    publicKey.Load(publicKeySeq);

    publicKeySeq.MessageEnd();
    // Đọc tin nhắn từ file
    std::string message;
    FileSource("message.txt", true, new StringSink(message));

    // Đọc chữ ký từ file
    std::string signature;
    HexDecoder decoder;
    decoder.Attach(new FileSource("signature.txt", true));
    decoder.MessageEnd();
    size_t signatureSize = decoder.MaxRetrievable();
    signature.resize(signatureSize);
    decoder.Get((byte*)&signature[0], signatureSize);

    // Xác thực chữ ký
    ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
    bool result = false;
    StringSource(message + signature, true,
        new SignatureVerificationFilter(verifier,
            new ArraySink((byte*)&result, sizeof(result))
        )
    );

    if (result) {
        std::cout << "Chu ky hop le" << std::endl;
    } else {
        std::cout << "Chu ky khong hop le" << std::endl;
    }

    return 0;
}