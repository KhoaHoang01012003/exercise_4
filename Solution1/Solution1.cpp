#include <gmpxx.h>
#include <gmp.h>
#include <iostream>

using namespace std;

void Rand_2Prime(mpz_t &p, mpz_t &q){
    // Khởi tạo bộ sinh số ngẫu nhiên
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    // Chọn option
    int flats;
    cout << "Số bao nhiêu byte (2), (8) và (32): ";
    cin >> flats;
    if(flats == 2){
        mpz_urandomb(p, state, 16);
        mpz_urandomb(q, state, 16);
    }

    else if(flats == 8){
        mpz_urandomb(p, state, 64);
        mpz_urandomb(q, state, 64);
    }
        
    else if(flats == 32){
        mpz_urandomb(p, state, 256);
        mpz_urandomb(q, state, 256);
    }
    else
        exit(1);
    
    mpz_nextprime(p, p);
    mpz_nextprime(q, q);

    //In 2 số p và q
    cout << "p = " << mpz_get_str(NULL, 10, p) << endl;
    cout << "q = " << mpz_get_str(NULL, 10, q) << endl;

    // Giải phóng bộ nhớ
    gmp_randclear(state);
}
void Input_Prime(mpz_t &p, mpz_t &q){
    //Nhập 2 số
    cout << "Nhập số p: ";
    mpz_inp_str(p, stdin, 10);
    cout << "Nhập số q: ";
    mpz_inp_str(q, stdin, 10);

    //Kiểm tra xem có phải số nguyên tố hay không. Nếu phải thì in ra.
    if(mpz_probab_prime_p(p, 0xFF))
        cout << "p = " << mpz_get_str(NULL, 10, p) << endl;
    else 
        cout << "Số p không phải là số nguyên tố."<< endl;

    if(mpz_probab_prime_p(q, 0xFF))
        cout << "q = " << mpz_get_str(NULL, 10, q) << endl;
    else 
        cout << "Số q không phải là số nguyên tố."<< endl;
}
int Modular_ax_mod_n(int a, int x, int n){
    int result = a % n;
    for(int i = 1; i < x; i++){
        result *= a % n;
        result %= n; 
    }
    return result % n;
}

int main()
{
    //Khởi tạo giá trị
    mpz_t p, q, gcd;
    mpz_init(q);
    mpz_init(p);
    mpz_init(gcd);

    //Chọn option
    int Op;
    cout << "Rand số nguyên tố(1); nhập số nguyên tố(2): ";
    cin >> Op;
    if(Op == 1){
        //Chọn ngẫu nhiên 2 số
        Rand_2Prime(p, q);
    }
    else if(Op == 2){
        //Nhập vào 2 số và kiểm tra xem số đó có phải số nguyên tố hay không 
        Input_Prime(p, q);
    }
    else
        exit(1);

    //Tính UCLN
    mpz_gcd (gcd, p, q);
    cout << "gcd = " << mpz_get_str(NULL, 10, gcd) << endl;
    
    //Khởi tạo và nhập 3 biến sau đó tính Modular
    int a, x, n;
    cout << "Nhập lần lượt: a, x, n \n";
    cin >> a >> x >> n;
    cout << "a^x mod p = " << Modular_ax_mod_n(a, x, n);

    //Giải phóng
    mpz_clear(gcd);
    mpz_clear(p);
    mpz_clear(q);
    return 0;
}
