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
    OID curve = secp256k1();
    DL_GroupParameters_EC<ECP> params(curve);
    const ECP::Point& generator = params.GetSubgroupGenerator();

    // Tạo private key
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    privateKey.Initialize(prng, params);

    // Tạo public key từ private key
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    // Lưu private key
    ByteQueue privateKeyBytes;
    privateKey.Save(privateKeyBytes);

    FileSink privateKeyFile("private_key.ber");
    privateKeyBytes.CopyTo(privateKeyFile);
    privateKeyFile.MessageEnd();

    // Lưu public key
    ByteQueue publicKeyBytes;
    publicKey.Save(publicKeyBytes);

    FileSink publicKeyFile("public_key.ber");
    publicKeyBytes.CopyTo(publicKeyFile);
    publicKeyFile.MessageEnd();

    // Đọc nội dung tin nhắn từ file
    std::string message;
    FileSource("message.txt", true, new StringSink(message));

    // Tạo chữ ký từ tin nhắn và khóa bí mật
    std::string signature;
    ECDSA<ECP, SHA256>::Signer signer(privateKey);
    StringSource(message, true,
        new SignerFilter(prng, signer,
            new StringSink(signature)
        )
    );

    // Lưu chữ ký vào file
    HexEncoder signatureEncoder(new FileSink("signature.txt"));
    signatureEncoder.Put((byte const*)signature.data(), signature.size());
    signatureEncoder.MessageEnd();

    std::cout << "Đã tạo private key, public key và chữ ký, và lưu vào các file tương ứng." << std::endl;
    return 0;
}