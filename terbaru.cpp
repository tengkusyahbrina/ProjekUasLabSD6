#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Produk {
    int id;
    string nama;
    string kategori;
    double harga;
};

string formatRupiah(double harga) {
    stringstream ss;
    ss << "Rp " << fixed << setprecision(0) << harga;
    return ss.str();
}

// Menggunakan Hash Map untuk menyimpan produk berdasarkan kategori
unordered_map<string, vector<Produk>> produkPerKategori;

vector<Produk> daftarProduk = {
    {1, "Smartphone Samsung Galaxy S23", "Elektronik", 12000000},
    {2, "Laptop ASUS ROG Zephyrus G14", "Elektronik", 25000000},
    {3, "TV LED LG 43 Inch", "Elektronik", 6500000},
    {4, "Earbuds Apple AirPods Pro", "Elektronik", 3500000},
    {5, "Kamera DSLR Canon EOS 90D", "Elektronik", 17000000},
    {6, "Sepeda MTB Polygon", "Fitness", 5000000},
    {7, "Dumbbell 5kg", "Fitness", 200000},
    {8, "Matras Yoga", "Fitness", 250000},
    {9, "Mesin Elliptical", "Fitness", 7500000},
    {10, "Resistance Band", "Fitness", 100000},
    {11, "Serum Wajah Vitamin C", "Kecantikan", 150000},
    {12, "Masker Wajah Aloe Vera", "Kecantikan", 75000},
    {13, "Lipstik Matte L'Oréal", "Kecantikan", 120000},
    {14, "Parfum Chanel", "Kecantikan", 2500000},
    {15, "Sabun Cuci Muka Himalaya", "Kecantikan", 35000},
    {16, "Roti Tawar Serba Roti", "Konsumsi", 15000},
    {17, "Kopi Arabica 100g", "Konsumsi", 50000},
    {18, "Mie Instan", "Konsumsi", 5000},
    {19, "Susu UHT Indomilk 1 Liter", "Konsumsi", 18000},
    {20, "Teh Kotak Sosro 500ml", "Konsumsi", 7500}
};

vector<int> keranjangBelanja;

void tampilkanHeader() {
    cout << "+-----+------------------------------------------+-------------------+----------------+\n";
    cout << "| ID  |                Nama Produk               |     Kategori      |     Harga      |\n";
    cout << "+-----+------------------------------------------+-------------------+----------------+\n";
}

void tampilkanProduk(const vector<Produk>& produk) {
    tampilkanHeader();
    for (const auto& p : produk) {
        cout << "| " << setw(3) << p.id << " | "
             << left << setw(40) << p.nama << " | "
             << left << setw(17) << p.kategori << " | "
             << right << setw(14) << formatRupiah(p.harga) << " |\n";
    }
    cout << "+-----+------------------------------------------+-------------------+----------------+\n";
}

void sortirHarga(vector<Produk>& produk, bool ascending) {
    sort(produk.begin(), produk.end(), [ascending](const Produk& a, const Produk& b) {
        return ascending ? a.harga < b.harga : a.harga > b.harga;
    });
}

void tambahKeKeranjang(int idProduk) {
    for (const auto& p : daftarProduk) {
        if (p.id == idProduk) {
            keranjangBelanja.push_back(idProduk);
            cout << "\nProduk \"" << p.nama << "\" berhasil ditambahkan ke keranjang.\n";
            return;
        }
    }
    cout << "\nProduk dengan ID " << idProduk << " tidak ditemukan.\n";
}

void tampilkanKeranjang() {
    if (keranjangBelanja.empty()) {
        cout << "\nKeranjang belanja kosong.\n";
        return;
    }
    cout << "\nIsi keranjang belanja:\n";
    tampilkanHeader();
    for (const auto& id : keranjangBelanja) {
        for (const auto& p : daftarProduk) {
            if (p.id == id) {
                cout << "| " << setw(3) << p.id << " | "
                     << left << setw(40) << p.nama << " | "
                     << left << setw(17) << p.kategori << " | "
                     << right << setw(14) << formatRupiah(p.harga) << " |\n";
            }
        }
    }
    cout << "+-----+------------------------------------------+-------------------+----------------+\n";
}

void inisialisasiHashMap() {
    for (const auto& produk : daftarProduk) {
        produkPerKategori[produk.kategori].push_back(produk);
    }
}

void tampilkanProdukBerdasarkanKategori(const string& kategori) {
    auto it = produkPerKategori.find(kategori);
    if (it != produkPerKategori.end()) {
        cout << "\nDaftar Produk Kategori \"" << kategori << "\":\n";
        tampilkanProduk(it->second);
    } else {
        cout << "\nTidak ada produk dalam kategori \"" << kategori << "\".\n";
    }
}

void menu() {
    int pilihan;
    do {
        cout << "\n=== SELAMAT DATANG DI TOKO TIGASETENGAH! ===\n";
        cout << "1. Tampilkan Semua Produk\n";
        cout << "2. Sortir Produk Berdasarkan Harga\n";
        cout << "3. Filter Produk Berdasarkan Kategori\n";
        cout << "4. Tambahkan Produk ke Keranjang\n";
        cout << "5. Tampilkan Keranjang Belanja\n";
        cout << "6. Keluar\n";
        cout << "Pilih opsi (1-6): ";
        cin >> pilihan;

        switch (pilihan) {
        case 1:
            cout << "\nDaftar Produk yang Tersedia di Toko TIGASETENGAH:\n";
            tampilkanProduk(daftarProduk);
            break;
        case 2: {
            int urut;
            cout << "\nPilih urutan harga:\n";
            cout << "1. Termurah ke Termahal\n";
            cout << "2. Termahal ke Termurah\n";
            cout << "Pilih opsi (1-2): ";
            cin >> urut;
            sortirHarga(daftarProduk, urut == 1);
            cout << "\nDaftar Produk Setelah Disortir:\n";
            tampilkanProduk(daftarProduk);
            break;
        }
        case 3: {
            string kategori;
            cout << "\nMasukkan kategori (Elektronik, Fitness, Kecantikan, Konsumsi, dll.): ";
            cin >> kategori;
            tampilkanProdukBerdasarkanKategori(kategori);
            break;
        }
        case 4: {
            int idProduk;
            cout << "\nMasukkan ID produk untuk ditambahkan ke keranjang: ";
            cin >> idProduk;
            tambahKeKeranjang(idProduk);
            break;
        }
        case 5:
            tampilkanKeranjang();
            break;
        case 6:
            cout << "\nTerima kasih telah menggunakan aplikasi. Selamat berbelanja!\n";
            break;
        default:
            cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 6);
}

int main() {
    inisialisasiHashMap();
    menu();
    return 0;
}
