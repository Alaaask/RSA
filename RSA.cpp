// 实现RSA算法
// p,q为大素数 128 bits整数
// 明文为10000以内的整数
// 生成密钥时：
// 输出p,q,e,d,n,Phi(n)到屏幕, (e,n)/(d,n)分别到文件public_key.txt/private_key.txt
// 加解密时：
// 例子中采用公钥(e,n)加密 私钥(d,n)解密
// 密钥从文件读入(原输出密钥的文件)
// 明/密文从屏幕输入/输出

#include<gmp.h>
#include<gmpxx.h>
#include<iostream>
#include<fstream>

using namespace std;

mpz_class generate_prime(unsigned bits);
void generate_keys(ostream &out, ostream &puKeyOut, ostream &prKeyOut);
void encrypt(mpz_class plaintext, const mpz_class e, const mpz_class n, ostream &out);
void decrypt(mpz_class ciphertext, const mpz_class d, const mpz_class n, ostream &out);

int main(void) {

    // 输入
    cout << "Enter g to generate keys, e to encrypt, d to decrypt, q to quit." << endl;
    char ch;
    while(cin >> ch) {

        if (ch == 'q') return 0;

        if (ch == 'g') {

            ofstream out1, out2;
            out1.open("./PublicKey.txt");
            out2.open("./PrivateKey.txt");

            generate_keys(cout, out1, out2);

            out1.close();
            out2.close();
            cout << endl;
        }

        if (ch == 'e') {

            mpz_class plaintext, e, n;

            cout << "Plaintext: ";
            cin >> plaintext;
            cout << "Read public key file." << endl;
            ifstream in("./PublicKey.txt");
            if (!in) {
                cout << "Wrong with the key file." << endl;
                return 1;
            }
            in >> e >> n;
            in.close();

            cout << "Ciphertext: ";
            encrypt(plaintext, e, n, cout);
            cout << endl;
        }

        if (ch == 'd') {

            mpz_class ciphertext, d, n;

            cout << "Ciphertext: ";
            cin >> ciphertext;
            cout << "Read private key file." << endl;
            ifstream in("./PrivateKey.txt");
            if (!in) {
                cout << "Wrong with the key file." << endl;
                return 1;
            }
            in >> d >> n;

            cout << "Plaintext: ";
            decrypt(ciphertext, d, n, cout);
            cout << endl;

        }   

    }

    return 0;
}


mpz_class generate_prime(unsigned bits) {

    // 生成128bit随机数
    mpz_class prime;
    gmp_randclass r(gmp_randinit_default);
    r.seed(unsigned(time(NULL)));

    // 检测是否素数
    // int mpz_probab_prime_p() 一定素数返回2 可能返回1 合数返回0
    while (!mpz_probab_prime_p(prime.get_mpz_t(), 20))
        prime = r.get_z_bits(128);

    return prime;
}


void generate_keys(ostream &out, ostream &puKeyOut, ostream &prKeyOut) {

    // 生成两个互质的大素数 128bits
    mpz_class p, q, n, Phi_n, e, d;
    p = generate_prime(128);
    q = generate_prime(128);
    while (p == q)
        q = generate_prime(128);

    // 计算n, Phi_n
    n = p * q;
    Phi_n = (p - 1) * (q - 1);

    // 选择e = 65537
    e = 65537;

    // 计算d
    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), Phi_n.get_mpz_t());

    // 输出密钥
    out << "p = " << p << endl << "q = " << q << endl
        << "e = " << e << endl << "d = " << d << endl
        << "n = " << n << endl << "Phi(n) = " << Phi_n << endl;

    puKeyOut << e << endl << n;

    prKeyOut << d << endl << n;
}


void encrypt(mpz_class plaintext, const mpz_class e, const mpz_class n, ostream &out) {

    mpz_class ciphertext;

    // 加密
    mpz_powm(ciphertext.get_mpz_t(), plaintext.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

    out << ciphertext << endl;
}


void decrypt(mpz_class ciphertext, const mpz_class d, const mpz_class n, ostream &out) {

    mpz_class plaintext;

    // 解密
    mpz_powm(plaintext.get_mpz_t(), ciphertext.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

    out << plaintext << endl;
}
