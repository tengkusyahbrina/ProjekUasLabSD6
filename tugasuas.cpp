#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <climits>
#include <ctime>
#include <limits> 
#define V 10

using namespace std;

struct Produk {
    int id;
    string nama;
    string kategori;
    double harga;
};

struct DetailTransaksi {
    int idProduk;
    int jumlah;
    double hargaSatuan;
};

struct RiwayatPembayaran {
    double totalBelanja;
    double ongkir;
    double totalBayar;
    string tanggal;
    string metodePembayaran;
    vector<DetailTransaksi> detail; 
};

const int MAX_RIWAYAT = 100;
const int MAX_PRODUK = 20;
RiwayatPembayaran riwayat[MAX_RIWAYAT];
int jumlahRiwayat = 0;

unordered_map<int, Produk> produkMap;
unordered_map<string, vector<Produk>> produkPerKategori;

int graph[V][V] = {
        {0, 0, 7, 4, 0, 0, 4, 0, 5, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 5, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 7, 0, 0},
        {0, 0, 0, 0, 9, 0, 0, 8, 0, 3},
        {0, 4, 0, 0, 2, 11, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 10, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 4, 0, 0}
};

string formatRupiah(double harga) {
    stringstream ss;
    ss << "Rp " << fixed << setprecision(0) << harga;
    return ss.str();
}

string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << put_time(ltm, "%d/%m/%Y %H:%M");
    return ss.str();
}

int getValidatedInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan angka antara " << min << " dan " << max << ": ";
        } else {
            break;
        }
    }
    return input;
}

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

int keranjangBelanja[21] = {0};

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

void tambahKeKeranjang(int idProduk) {
    if (idProduk < 1 || idProduk > 20) {
        cout << "\nProduk dengan ID " << idProduk << " tidak ditemukan.\n";
        return;
    }
    keranjangBelanja[idProduk]++;
    cout << "\nProduk \"" << produkMap[idProduk].nama << "\" berhasil ditambahkan ke keranjang.\n";
}

void tampilkanKeranjang() {
    bool keranjangKosong = true;
    double totalHarga = 0;

    cout << "\nIsi keranjang belanja Anda:\n";
    cout << "+-----+------------------------------------------+------------+----------------+\n";
    cout << "| ID  |                Nama Produk               |   Jumlah   |     Subtotal   |\n";
    cout << "+-----+------------------------------------------+------------+----------------+\n";

    for (int id = 1; id <= 20; ++id) {
        if (keranjangBelanja[id] > 0) {
            keranjangKosong = false;
            double subtotal = keranjangBelanja[id] * produkMap[id].harga;
            totalHarga += subtotal;

            cout << "| " << setw(3) << id << " | "
                 << left << setw(40) << produkMap[id].nama << " | "
                 << setw(10) << keranjangBelanja[id] << " | "
                 << right << setw(14) << formatRupiah(subtotal) << " |\n";
        }
    }

    if (keranjangKosong) {
        cout << "\nKeranjang belanja kosong.\n";
    } else {
        cout << "+-----+------------------------------------------+------------+----------------+\n";
        cout << "Total Harga: " << formatRupiah(totalHarga) << "\n";
    }
}

void hapusDariKeranjang(int idProduk) {
    if (idProduk < 1 || idProduk > 20) {
        cout << "\nProduk dengan ID " << idProduk << " tidak ditemukan.\n";
        return;
    }
    if (keranjangBelanja[idProduk] == 0) {
        cout << "\nProduk \"" << produkMap[idProduk].nama << "\" tidak ada di keranjang.\n";
    } else {
        keranjangBelanja[idProduk]--;
        cout << "\nProduk \"" << produkMap[idProduk].nama << "\" berhasil dihapus dari keranjang.\n";
    }
}

void cariProdukByID() {
    int id;
    cout << "\nMasukkan ID produk yang ingin dicari: ";
    cin >> id;

    if (produkMap.find(id) != produkMap.end()) {
        cout << "\nProduk ditemukan:\n";
        tampilkanProduk({produkMap[id]});
    } else {
        cout << "\nProduk dengan ID " << id << " tidak ditemukan.\n";
    }
}

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int graph[V][V], int src, int dist[], bool sptSet[]) {
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] != 0 && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

void hitungBiayaPengiriman(int dist[], int biayaPerKm) {
    cout << "\nBiaya Pengiriman (Per Kilometer Rp " << biayaPerKm << "):\n";
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX) {
            cout << "Ke vertex " << i << ": Tidak dapat dijangkau\n";
        } else {
            int biaya = dist[i] * biayaPerKm;
            cout << "Ke vertex " << i << ": " << dist[i] << " km, Biaya: Rp " << biaya << "\n";
        }
    }
}

void menuPengiriman() {
    int pilihan, tujuan;
    int dist[V];
    bool sptSet[V];

    do {
        cout << "\n=== MENU PENGIRIMAN ===\n";
        cout << "1. Hitung Biaya Pengiriman\n";
        cout << "2. Kembali ke Menu Utama\n";
        cout << "Pilih opsi (1-2): ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                cout << "Masukkan titik tujuan (0-9): ";
                cin >> tujuan;

                dijkstra(graph, 0, dist, sptSet);

                if (dist[tujuan] == INT_MAX) {
                    cout << "Titik tujuan tidak dapat dijangkau.\n";
                } else {
                    int biaya = dist[tujuan] * 3000;
                    cout << "Jarak terpendek dari 0 ke " << tujuan << ": " << dist[tujuan] << " km\n";
                    cout << "Biaya Pengiriman: Rp " << biaya << "\n";
                }
                break;
            }
            case 2:
                cout << "Kembali ke Menu Utama.\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 2);
}

void pembayaran() {
    double totalHarga = 0;
    for (int id = 1; id <= MAX_PRODUK; ++id) {
        totalHarga += keranjangBelanja[id] * produkMap[id].harga;
    }

    if (totalHarga == 0) {
        cout << "\nKeranjang Anda kosong. Silakan tambahkan produk sebelum melakukan pembayaran.\n";
        return;
    }

     int tujuan;
    cout << "\nMasukkan titik tujuan pengiriman (0-9): ";
    cin >> tujuan;

    int dist[V];
    bool sptSet[V];
    dijkstra(graph, 0, dist, sptSet);

    if (dist[tujuan] == INT_MAX) {
        cout << "Titik tujuan tidak dapat dijangkau. Pembayaran dibatalkan.\n";
        return;
    }

    double ongkir = dist[tujuan] * 3000;
    double totalBayar = totalHarga + ongkir;

    if (jumlahRiwayat < MAX_RIWAYAT) {
        riwayat[jumlahRiwayat].totalBelanja = totalHarga;
        riwayat[jumlahRiwayat].ongkir = ongkir;
        riwayat[jumlahRiwayat].totalBayar = totalBayar;
        riwayat[jumlahRiwayat].tanggal = getCurrentTime();
        riwayat[jumlahRiwayat].metodePembayaran = "Metode Pembayaran";

        for (int id = 1; id <= MAX_PRODUK; ++id) {
            if (keranjangBelanja[id] > 0) {
                double subtotal = keranjangBelanja[id] * produkMap[id].harga;
                riwayat[jumlahRiwayat].detail.push_back({id, keranjangBelanja[id], produkMap[id].harga});
            }
        }
        jumlahRiwayat++;
        cout << "\nPembayaran berhasil!\n";
        cout << "Total Belanja: " << formatRupiah(totalHarga) << "\n";
        cout << "Biaya Pengiriman: " << formatRupiah(ongkir) << "\n";
        cout << "Total yang Harus Dibayar: " << formatRupiah(totalBayar) << "\n";
        fill(begin(keranjangBelanja), end(keranjangBelanja), 0);
    } else {
        cout << "\nRiwayat pembayaran penuh. Tidak dapat menyimpan pembayaran baru.\n";
    }
}

void tampilkanRiwayat() {
    if (jumlahRiwayat == 0) {
        cout << "\nBelum ada riwayat pembayaran.\n";
        return;
    }

    cout << "\n=== RIWAYAT PEMBAYARAN ===\n";
    cout << "+-----+----------------+----------------+----------------+-------------------+\n";
    cout << "| No  | Total Belanja  |     Ongkir     |  Total Bayar   |      Tanggal      |\n";
    cout << "+-----+----------------+----------------+----------------+-------------------+\n";

    for (int i = 0; i < jumlahRiwayat; ++i) {
        cout << "| " << setw(3) << i + 1 << " | "
             << right << setw(14) << formatRupiah(riwayat[i].totalBelanja) << " | "
             << setw(14) << formatRupiah(riwayat[i].ongkir) << " | "
             << setw(14) << formatRupiah(riwayat[i].totalBayar) << " | "
             << setw(17) << riwayat[i].tanggal << " |\n";
    }

    cout << "+-----+----------------+----------------+----------------+-------------------+\n";
}

void tampilkanDetailTransaksi(int noTransaksi) {
    if (noTransaksi < 1 || noTransaksi > jumlahRiwayat) {
        cout << "\nNomor transaksi tidak valid.\n";
        return;
    }

    cout << "\nDetail Transaksi Nomor " << noTransaksi << ":\n";
    cout << "Tanggal: " << riwayat[noTransaksi - 1].tanggal << endl;
    cout << "+-----+------------------------------------------+------------+----------------+\n";
    cout << "|  ID |                Nama Produk               | Jumlah |     Subtotal   |\n";
    cout << "+-----+------------------------------------------+------------+----------------+\n";

    unordered_map<int, Produk> transactionProductMap;
    for (const auto& dt : riwayat[noTransaksi - 1].detail) {
        transactionProductMap[dt.idProduk] = produkMap[dt.idProduk];
    }

    for (const auto& dt : riwayat[noTransaksi - 1].detail) {
        double subtotal = dt.jumlah * dt.hargaSatuan;
        cout << "| " << setw(3) << dt.idProduk << " | "
             << left << setw(40) << transactionProductMap[dt.idProduk].nama << " | "
             << setw(10) << dt.jumlah << " | "
             << right << setw(14) << formatRupiah(subtotal) << " |\n";
    }

    cout << "+-----+------------------------------------------+------------+----------------+\n";
    cout << "Total Belanja: " << formatRupiah(riwayat[noTransaksi - 1].totalBelanja) << "\n";
    cout << "Biaya Pengiriman: " << formatRupiah(riwayat[noTransaksi - 1].ongkir) << "\n";
    cout << "Total yang Harus Dibayar: " << formatRupiah(riwayat[noTransaksi - 1].totalBayar) << "\n";
    cout << "Tekan Enter untuk kembali ke menu utama..." << endl;
    cin.get();
    cin.get();
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
        cout << "6. Cari Produk Berdasarkan ID\n";
        cout << "7. Hapus Produk dari Keranjang\n";
        cout << "8. Hitung Biaya Pengiriman\n";
        cout << "9. Lakukan Pembayaran\n";
        cout << "10. Tampilkan Riwayat Pembayaran\n";
        cout << "11. Tampilkan Detail Riwayat Pembayaran\n";
        cout << "12. Keluar\n";
        cout << "Pilih opsi (1-12): ";
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
                vector<Produk> produkSorted = daftarProduk; 
                sortirHarga(produkSorted, urut == 1);
                cout << "\nDaftar Produk Setelah Disortir:\n";
                tampilkanProduk(produkSorted);
                break;
            }
            case 3: {
                string kategori;
            cout << "\nMasukkan kategori (Elektronik, Fitness, Kecantikan, Konsumsi): ";
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
                cariProdukByID();
                break;
            case 7: {
            int idProduk;
            cout << "\nMasukkan ID produk untuk dihapus dari keranjang: ";
            cin >> idProduk;
            hapusDariKeranjang(idProduk);
            break;
        }
            case 8:
                menuPengiriman();
                break;
            case 9:
                pembayaran();
                break;
            case 10:
                tampilkanRiwayat();
                break;
            case 11: {
                int noTransaksi;
                cout << "Masukkan nomor transaksi yang ingin dilihat detailnya (1-" << jumlahRiwayat << "): ";
                noTransaksi = getValidatedInput(1, jumlahRiwayat);
                tampilkanDetailTransaksi(noTransaksi);
                break;
            }
            case 12:
                cout << "\nTerima kasih telah berbelanja di Toko TIGASETENGAH. Sampai jumpa lagi!\n";
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 12);
}

int main() {
    for (const auto& p : daftarProduk) {
        produkMap[p.id] = p;
    }
    inisialisasiHashMap();
    menu();
    return 0;
}