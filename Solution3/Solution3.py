import gmpy2 
import random
import numbers

def is_probable_prime(n, k=10):
    #Kiểm tra số nguyên tố n theo thuật toán Miller-Rabin
    if n == 2 or n == 3:
        return True
    if n <= 1 or n % 2 == 0:
        return False

    # Phân tích n-1 thành d.2^r
    d = n - 1
    r = 0
    while d % 2 == 0:
        d //= 2
        r += 1

    # Kiểm tra tính nguyên tố của n với độ chính xác k
    for i in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for j in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_prime(bits):
    #Tạo số nguyên tố ngẫu nhiên với số bit được chỉ định.
    while True:
        p = random.getrandbits(bits)
        if is_probable_prime(p):
            return p

def get_n(p, q):
    return p*q

def get_phi_n(p, q):
    return (p-1) * (q-1)

def get_d(e, phi):
    for i in range(1, phi):
        d = (1 + i * phi) // e
        if d * e % phi == 1:
            return int(d)
    return 0

def encrypt_RSA(plaintext, public_key):
    n, e = public_key
    # Chuyển chuỗi string sang int theo từng bytes
    m = 0
    for char in plaintext:
        m = (m << 8) + ord(char)
        
    # Mã hóa bằng public key
    c = pow(m, e, n)
    return c

def decrypt_RSA(ciphertext, private_key):
    n, d = private_key
    # Giải mã bằng private key
    m = pow(ciphertext, d, n)
    # Chuyển từ int thành string
    plaintext = ""
    while m > 0:
        plaintext = chr(m & 0xFF) + plaintext
        m = m >> 8
    return plaintext

def main():
    print("RSA!")
    #Thực hiện vòng lặp liên tục nếu nhập y kết thúc khi nhập n
    while True:
        #Nhập dữ liệu
        while True:
            try:
                #Chọn option
                option = int(input("Enter random (p, q, e) (1) or enter (p, q, e) (2): "))
                if option == 2:
                    p = int(input("Enter p: "))    
                    q = int(input("Enter q: "))
                    e = int(input("Enter e: "))
                    if not (is_probable_prime(p) and is_probable_prime(q) and is_probable_prime(e)):
                        raise ValueError("Invalid input: p, q, e must all be prime numbers")
                    break  
                elif option == 1:
                    p = generate_prime(1024)    
                    q = generate_prime(1024)
                    e = 65537   #Số e thường được sử dụng
                    break
            except ValueError:
                print("Please enter a valid number!")
            except:
                print("An error occurred. Please try again.")
        
        #Public_Key(n, e)
        public_key = (get_n(p,q), e)
        #Private_Key(n, d)
        private_key = (get_n(p,q), get_d(e, get_phi_n(p, q)))
        print("p = ", p)
        print("q = ", q)
        print("e = ", e)
        print("n = ", get_n(p, q))
        print("phi = ", get_phi_n(p, q))
        print("d = ", get_d(e, get_phi_n(p, q)))

        plaintext = input("Enter plaintext: ")
        
        # Encrypt 
        ciphertext = encrypt_RSA(plaintext, public_key)
        print("Encrypted message:", ciphertext)

        # Decrypt 
        decrypted_plaintext = decrypt_RSA(ciphertext, private_key)
        print("Decrypted message:", decrypted_plaintext)
        
        #Chọn option lập lại hoặc không
        option =input("Do you want to continue? (y/n): ")
        if option == "y" or option == "Y":
            continue
        else:
            exit()

            
if __name__=="__main__":
    main()