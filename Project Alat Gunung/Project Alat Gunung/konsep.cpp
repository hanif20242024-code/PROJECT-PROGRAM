#define _CRT_SECURE_NO_WARNINGS // Menghindari warning localtime di Visual Studio
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <random>

using namespace std;

// ==========================================
// CLASS 1: BARANG
// ==========================================
class Barang {
public:
    string kodeBarang;
    string namaBarang;
    string merk;
    string spesifikasi;
    double hargaPerHari;
    int stok;

    Barang() {}
    Barang(string kb, string nb, string m, string spek, double hrg, int stk) {
        kodeBarang = kb;
        namaBarang = nb;
        merk = m;
        spesifikasi = spek;
        hargaPerHari = hrg;
        stok = stk;
    }

    void updateStok(int jumlah) {
        stok += jumlah;
    }

    void ubahDetail(string nama, string m, double harga) {
        namaBarang = nama;
        merk = m;
        hargaPerHari = harga;
    }
};

// ==========================================
// CLASS 2: TRANSAKSI (Kolaborasi Putra, Gibran, Hanif)
// ==========================================
class Transaksi {
public:
    // --- Bagian Putra ---
    string kodePemesan;
    string namaPemesan;
    string noTelp;
    string jaminan;

    // --- Bagian Gibran ---
    Barang barangDisewa;
    int jumlahSewa;
    int durasiHari;
    string tanggalPinjam;
    string tanggalKembali;

    // --- Bagian Hanif ---
    double totalHarga;
    string metodeBayar;
    double uangBayar;
    double kembalian;

    void hitungWaktuOtomatis() {
        auto sekarang = chrono::system_clock::now();
        time_t waktu_c = chrono::system_clock::to_time_t(sekarang);
        char buf1[50];
        strftime(buf1, sizeof(buf1), "%d-%m-%Y %H:%M", localtime(&waktu_c));
        this->tanggalPinjam = string(buf1);

        auto kembali = sekarang + chrono::hours(24 * this->durasiHari);
        time_t kembali_c = chrono::system_clock::to_time_t(kembali);
        char buf2[50];
        strftime(buf2, sizeof(buf2), "%d-%m-%Y %H:%M", localtime(&kembali_c));
        this->tanggalKembali = string(buf2);
    }

    void hitungTotalBiaya() {
        totalHarga = barangDisewa.hargaPerHari * jumlahSewa * durasiHari;
    }

    void tampilkanRincian() {
        cout << "\n=======================================";
        cout << "\n         RINCIAN PESANAN ANDA        ";
        cout << "\n=======================================\n";
        cout << "Kode Pemesan : " << kodePemesan << "\n";
        cout << "Nama Penyewa : " << namaPemesan << "\n";
        cout << "Barang       : " << barangDisewa.namaBarang << " (" << barangDisewa.merk << " - " << barangDisewa.spesifikasi << ")\n";
        cout << "Jumlah       : " << jumlahSewa << " unit\n";
        cout << "Durasi       : " << durasiHari << " hari\n";
        cout << "Tgl Pinjam   : " << tanggalPinjam << "\n";
        cout << "Tgl Kembali  : " << tanggalKembali << "\n";
        cout << "---------------------------------------\n";
        cout << "TOTAL HARGA  : Rp. " << fixed << setprecision(0) << totalHarga << "\n";
        cout << "=======================================\n";
    }

    void cetakNotaTxt() {
        ofstream file("Nota_" + kodePemesan + ".txt");
        if (file.is_open()) {
            file << "=======================================\n";
            file << "         NOTA SEWA ALAT GUNUNG         \n";
            file << "=======================================\n";
            file << "Kode Pemesan : " << kodePemesan << "\n";
            file << "Nama Penyewa : " << namaPemesan << "\n";
            file << "No Telfon    : " << noTelp << "\n";
            file << "Jaminan      : " << jaminan << "\n";
            file << "---------------------------------------\n";
            file << "Barang       : " << barangDisewa.namaBarang << " " << barangDisewa.merk << "\n";
            file << "Spesifikasi  : " << barangDisewa.spesifikasi << "\n";
            file << "Jumlah       : " << jumlahSewa << " unit\n";
            file << "Durasi       : " << durasiHari << " hari\n";
            file << "Tgl Pinjam   : " << tanggalPinjam << "\n";
            file << "Tgl Kembali  : " << tanggalKembali << "\n";
            file << "---------------------------------------\n";
            file << "Total Bayar  : Rp. " << fixed << setprecision(0) << totalHarga << "\n";
            file << "Metode Bayar : " << metodeBayar << "\n";
            if (metodeBayar == "Cash" || metodeBayar == "CASH") {
                file << "Uang Dibayar : Rp. " << uangBayar << "\n";
                file << "Kembalian    : Rp. " << kembalian << "\n";
            }
            file << "=======================================\n";
            file << "      TERIMA KASIH TELAH MENYEWA       \n";
            file.close();
            cout << "[INFO] Nota berhasil dicetak (Nota_" << kodePemesan << ".txt)\n";
        }
    }
};

// ==========================================
// CLASS 3: ADMIN (Bagian Nayo)
// ==========================================
class Admin {
private:
    string passwordMaster = "adminGunung123";

public:
    bool login() {
        string inputPass;
        cout << "Masukkan Password Admin: ";
        cin >> inputPass;
        return (inputPass == passwordMaster);
    }

    void lihatStok(const vector<Barang>& daftarBarang) {
        cout << "\n=== STOCK BARANG ===\n";
        for (const auto& b : daftarBarang) {
            cout << b.kodeBarang << " | " << b.namaBarang << " " << b.merk << " (" << b.spesifikasi << ") - Stok: " << b.stok << " - Rp." << b.hargaPerHari << "/hari\n";
        }
    }

    void restokBarang(vector<Barang>& daftarBarang) {
        string kode;
        int jml;
        lihatStok(daftarBarang);
        cout << "Masukkan Kode Barang yang ditambah/dikurangi stoknya: "; cin >> kode;
        cout << "Masukkan jumlah (gunakan minus untuk mengurangi): "; cin >> jml;

        for (auto& b : daftarBarang) {
            if (b.kodeBarang == kode) {
                b.updateStok(jml);
                cout << "[SUKSES] Stok berhasil diupdate. Stok sekarang: " << b.stok << "\n";
                return;
            }
        }
        cout << "[GAGAL] Kode barang tidak ditemukan.\n";
    }

    void tambahJenisBarang(vector<Barang>& daftarBarang) {
        string kb, nb, m, spek; double hrg; int stk;
        cout << "Kode Barang Baru: "; cin >> kb;
        cout << "Nama Barang: "; cin.ignore(); getline(cin, nb);
        cout << "Merk: "; getline(cin, m);
        cout << "Spesifikasi: "; getline(cin, spek);
        cout << "Harga / Hari: "; cin >> hrg;
        cout << "Stok Awal: "; cin >> stk;
        daftarBarang.push_back(Barang(kb, nb, m, spek, hrg, stk));
        cout << "[SUKSES] Jenis barang baru berhasil ditambahkan.\n";
    }

    void monitoringPenyewa(vector<Transaksi>& daftarAktif, vector<Transaksi>& daftarSelesai, vector<Barang>& daftarBarang) {
        if (daftarAktif.empty()) {
            cout << "Tidak ada penyewa aktif.\n";
            return;
        }
        cout << "\n=== DAFTAR PENYEWA AKTIF ===\n";
        for (size_t i = 0; i < daftarAktif.size(); i++) {
            cout << i + 1 << ". " << daftarAktif[i].kodePemesan << " - " << daftarAktif[i].namaPemesan
                << " | " << daftarAktif[i].barangDisewa.namaBarang << " (" << daftarAktif[i].jumlahSewa << " unit) | Durasi: " << daftarAktif[i].durasiHari << " hari\n";
        }

        cout << "\nMenu Monitoring:\n1. Tandai Barang Kembali\n2. Ubah Data Penyewa\n3. Kembali\nPilih: ";
        int pil; cin >> pil;
        if (pil == 1) {
            int idx; cout << "Pilih nomor urut penyewa yang mengembalikan barang: "; cin >> idx;
            if (idx > 0 && idx <= daftarAktif.size()) {
                // Kembalikan stok
                for (auto& b : daftarBarang) {
                    if (b.kodeBarang == daftarAktif[idx - 1].barangDisewa.kodeBarang) {
                        b.updateStok(daftarAktif[idx - 1].jumlahSewa);
                    }
                }
                // Pindah ke log selesai
                daftarSelesai.push_back(daftarAktif[idx - 1]);
                daftarAktif.erase(daftarAktif.begin() + (idx - 1));
                cout << "[SUKSES] Barang dikembalikan. Stok bertambah dan masuk ke Log Sewa.\n";
            }
        }
    }

    void monitoringLogSewa(const vector<Transaksi>& daftarSelesai) {
        if (daftarSelesai.empty()) {
            cout << "Belum ada riwayat penyewaan selesai.\n";
            return;
        }
        cout << "\n=== LOG PENYEWAAN SELESAI ===\n";
        for (const auto& log : daftarSelesai) {
            cout << log.kodePemesan << " | " << log.namaPemesan << " | " << log.barangDisewa.namaBarang << " (" << log.jumlahSewa << " unit) | Dikembalikan\n";
        }
    }
};

// ==========================================
// CLASS 4: SISTEM SEWA (Engine Utama)
// ==========================================
class SistemSewa {
private:
    vector<Barang> databaseBarang;
    vector<Transaksi> transaksiAktif;
    vector<Transaksi> logSelesai;
    Admin adminSistem;

    // Fungsi helper random kode generator (Bagian Putra)
    string generateKode(string prefix) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1000, 9999);
        return prefix + to_string(dis(gen));
    }

public:
    void inisialisasiDataAwal() {
        databaseBarang.push_back(Barang("S01", "Sepatu Gunung", "Eiger", "Ukuran 42", 25000, 10));
        databaseBarang.push_back(Barang("T01", "Tenda Dome", "Consina", "Kapasitas 4 Org", 50000, 5));
        databaseBarang.push_back(Barang("C01", "Carrier", "Arei", "60 Liter", 35000, 8));
    }

    void lihatKatalog() {
        cout << "\n=======================================\n";
        cout << "         KATALOG ALAT GUNUNG           \n";
        cout << "=======================================\n";
        for (const auto& b : databaseBarang) {
            cout << "Kode  : " << b.kodeBarang << "\n";
            cout << "Nama  : " << b.namaBarang << " " << b.merk << "\n";
            cout << "Spek  : " << b.spesifikasi << "\n";
            cout << "Harga : Rp. " << b.hargaPerHari << " / hari\n";
            cout << "Stok  : " << b.stok << " unit\n";
            cout << "---------------------------------------\n";
        }
    }

    void fiturSewaBarang() {
        Transaksi trxBaru;

        // --- TUGAS PUTRA ---
        cout << "\n=== INPUT DATA PENYEWA ===\n";
        trxBaru.kodePemesan = generateKode("TRX");
        cout << "Kode Pemesan (Otomatis): " << trxBaru.kodePemesan << "\n";

        cin.ignore(); // Membersihkan buffer
        cout << "Nama Lengkap: "; getline(cin, trxBaru.namaPemesan);
        cout << "No Telp     : "; getline(cin, trxBaru.noTelp);
        cout << "Jaminan     : "; getline(cin, trxBaru.jaminan);

        // --- TUGAS GIBRAN ---
        bool yakin = false;
        while (!yakin) {
            lihatKatalog();
            string kodePilih;
            cout << "Masukkan Kode Barang yang disewa: "; cin >> kodePilih;

            bool ketemu = false;
            for (auto& b : databaseBarang) {
                if (b.kodeBarang == kodePilih) {
                    trxBaru.barangDisewa = b;
                    ketemu = true;
                    break;
                }
            }

            if (!ketemu) {
                cout << "[ERROR] Kode barang tidak ditemukan. Ulangi.\n";
                continue;
            }

            cout << "Jumlah yang disewa: "; cin >> trxBaru.jumlahSewa;
            if (trxBaru.jumlahSewa > trxBaru.barangDisewa.stok) {
                cout << "[ERROR] Stok tidak mencukupi!\n";
                continue;
            }

            cout << "Durasi Sewa (Hari): "; cin >> trxBaru.durasiHari;

            // Konfirmasi (Gibran)
            cout << "\nKonfirmasi: Anda memilih " << trxBaru.barangDisewa.namaBarang << " sebanyak " << trxBaru.jumlahSewa << " buah.\n";
            cout << "Apakah Anda yakin? (Y/N): ";
            char cYakin; cin >> cYakin;
            if (cYakin == 'Y' || cYakin == 'y') {
                yakin = true;
                // Kurangi stok di database utama
                for (auto& b : databaseBarang) {
                    if (b.kodeBarang == trxBaru.barangDisewa.kodeBarang) {
                        b.updateStok(-trxBaru.jumlahSewa);
                    }
                }
            }
            else {
                cout << "Kembali ke pemilihan barang...\n";
            }
        }

        // --- TUGAS HANIF ---
        trxBaru.hitungWaktuOtomatis();
        trxBaru.hitungTotalBiaya();
        trxBaru.tampilkanRincian();

        cout << "Metode Pembayaran (Cash/TF): "; cin >> trxBaru.metodeBayar;
        if (trxBaru.metodeBayar == "Cash" || trxBaru.metodeBayar == "CASH" || trxBaru.metodeBayar == "cash") {
            do {
                cout << "Masukkan nominal uang bayar: Rp. "; cin >> trxBaru.uangBayar;
                if (trxBaru.uangBayar < trxBaru.totalHarga) {
                    cout << "[ERROR] Uang kurang!\n";
                }
            } while (trxBaru.uangBayar < trxBaru.totalHarga);

            trxBaru.kembalian = trxBaru.uangBayar - trxBaru.totalHarga;
            cout << "Kembalian Anda: Rp. " << trxBaru.kembalian << "\n";
        }
        else {
            cout << "Silahkan Transfer ke Rekening BCA: 1234567890 a.n Rental Gunung\n";
            trxBaru.uangBayar = trxBaru.totalHarga;
            trxBaru.kembalian = 0;
        }

        trxBaru.cetakNotaTxt();
        transaksiAktif.push_back(trxBaru); // Simpan ke database transaksi
    }

    void menuAdmin() {
        if (!adminSistem.login()) {
            cout << "[Akses Ditolak] Password Salah!\n";
            return;
        }

        int pil;
        do {
            cout << "\n=== PANEL ADMIN ===\n";
            cout << "1. Lihat Stok Barang\n";
            cout << "2. Tambah/Kurangi Stok\n";
            cout << "3. Tambah Jenis Barang Baru\n";
            cout << "4. Monitoring Penyewa Aktif (Kembalikan Barang)\n";
            cout << "5. Monitoring Log Sewa (Selesai)\n";
            cout << "6. Keluar Panel Admin\n";
            cout << "Pilih: "; cin >> pil;

            switch (pil) {
            case 1: adminSistem.lihatStok(databaseBarang); break;
            case 2: adminSistem.restokBarang(databaseBarang); break;
            case 3: adminSistem.tambahJenisBarang(databaseBarang); break;
            case 4: adminSistem.monitoringPenyewa(transaksiAktif, logSelesai, databaseBarang); break;
            case 5: adminSistem.monitoringLogSewa(logSelesai); break;
            case 6: cout << "Keluar dari menu admin...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
            }
        } while (pil != 6);
    }

    void menuUtama() {
        int pilihan;
        do {
            cout << "\n=======================================\n";
            cout << "       SISTEM RENTAL ALAT GUNUNG       \n";
            cout << "=======================================\n";
            cout << "1. Sewa Barang\n";
            cout << "2. Melihat Katalog Barang\n";
            cout << "3. Admin\n";
            cout << "4. Keluar\n";
            cout << "Pilihan Anda: ";
            cin >> pilihan;

            switch (pilihan) {
            case 1: fiturSewaBarang(); break;
            case 2: lihatKatalog(); break;
            case 3: menuAdmin(); break;
            case 4: cout << "Terima kasih telah menggunakan sistem ini!\n"; break;
            default: cout << "Pilihan tidak valid, coba lagi.\n";
            }
        } while (pilihan != 4);
    }
};

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    SistemSewa engine;
    engine.inisialisasiDataAwal();
    engine.menuUtama();
    return 0;
}