# Generate RSA keys
from Crypto.Util.number import getPrime

def get_n(p, q):
    return p * q

def get_phi_n(p, q):
    return (p - 1) * (q - 1)

def get_d(e, phi_n):
    return pow(e, -1, phi_n)

def encrypt_RSA(plaintext, public_key):
    """Encrypts a plaintext message using RSA with the given public key.
    The plaintext can be a string or a numeric value."""
    
    n, e = public_key
    if isinstance(plaintext, str):
        # Convert string to integer
        m = 0
        for char in plaintext:
            m = (m << 8) + ord(char)
    else:
        # plaintext is already a number
        m = plaintext
    # Encrypt the message using public key
    c = pow(m, e, n)
    # Return the ciphertext
    return c

def decrypt_RSA(ciphertext, private_key):
    """Decrypts a ciphertext message using RSA with the given private key.
    Returns the decrypted plaintext as a string."""
    
    n, d = private_key
    # Decrypt the message using private key
    m = pow(ciphertext, d, n)
    # Convert the decrypted integer back to string
    plaintext = ""
    while m > 0:
        plaintext = chr(m & 0xFF) + plaintext
        m = m >> 8
    # Return the decrypted plaintext
    return plaintext

p = getPrime(1024)
q = getPrime(1024)
e = 65537

public_key = (get_n(p, q), e)
private_key = (get_n(p, q), get_d(e, get_phi_n(p, q)))

# Encrypt a message using RSA
plaintext = "Hiep"
ciphertext = encrypt_RSA(plaintext, public_key)
print("Encrypted message:", ciphertext)

# Decrypt the message using RSA
decrypted_plaintext = decrypt_RSA(ciphertext, private_key)
print("Decrypted message:", decrypted_plaintext)