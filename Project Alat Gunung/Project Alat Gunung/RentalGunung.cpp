#include "RentalGunung.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <random>
#include <cstdlib> 
#include <conio.h> 

using namespace std;

// =========================================================================
// FUNGSI HELPER UNTUK VALIDASI ANGKA & NOMOR TELEPON
// =========================================================================

int inputInt(string prompt) {

    int angka;
    while (true) {
        cout << prompt;
        if (cin >> angka) {
            if (angka <= 0) {
                cout << "[ERROR] Input tidak boleh negatif atau 0! Harap masukkan angka bulat positif.\n";
                continue;
            }
            else {
                return angka;
            }
        }
        else {
            cout << "[ERROR] Input tidak valid! Harap masukkan angka bulat.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

double inputDouble(string prompt) {
    double angka;
    while (true) {
        cout << prompt;
        if (cin >> angka) {
            return angka;
        }
        else {
            cout << "[ERROR] Input tidak valid! Harap masukkan angka nominal yang benar.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

string inputNoTelp(string prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "[ERROR] Nomor telepon tidak boleh kosong!\n";
            continue;
        }
        bool semuaAngka = true;
        for (char c : input) {
            if (c < '0' || c > '9') {
                semuaAngka = false;
                break;
            }
        }
        if (semuaAngka) {
            return input;
        }
        else {
            cout << "[ERROR] Nomor telepon hanya boleh berisi angka murni (0-9), tanpa spasi/huruf/simbol!\n";
        }
    }
}
// =========================================================================


// === IMPLEMENTASI CLASS BARANG ===
Barang::Barang() {}
Barang::Barang(string kb, string nb, string m, string spek, double hrg, int stk) {
    kodeBarang = kb; namaBarang = nb; merk = m; spesifikasi = spek; hargaPerHari = hrg; stok = stk;
}
void Barang::updateStok(int jumlah) { stok += jumlah; }
void Barang::ubahDetail(string nama, string m, double harga, string k, string s) { 
    namaBarang = nama;
    merk = m;
    hargaPerHari = harga;
    kodeBarang = k;
    spesifikasi = s;
}

// === IMPLEMENTASI CLASS TRANSAKSI ===
void Transaksi::hitungWaktuOtomatis() {
    auto sekarang = chrono::system_clock::now();
    time_t waktu_c = chrono::system_clock::to_time_t(sekarang);
    char buf1[50];
    strftime(buf1, sizeof(buf1), "%d-%m-%Y %H:%M", localtime(&waktu_c));
    this->tanggalPinjam = string(buf1);

    int maxDurasi = 0;
    for (int d : listDurasiHari) {
        if (d > maxDurasi) maxDurasi = d;
    }

    auto kembali = sekarang + chrono::hours(24 * maxDurasi);
    time_t kembali_c = chrono::system_clock::to_time_t(kembali);
    char buf2[50];
    strftime(buf2, sizeof(buf2), "%d-%m-%Y %H:%M", localtime(&kembali_c));
    this->tanggalKembali = string(buf2);
}

void Transaksi::hitungTotalBiaya() {
    totalHarga = 0;
    for (size_t i = 0; i < listBarang.size(); i++) {
        totalHarga += listBarang[i].hargaPerHari * listJumlahSewa[i] * listDurasiHari[i];
    }
}

void Transaksi::tampilkanRincian() {
    cout << "\n=======================================";
    cout << "\n         RINCIAN PESANAN ANDA        ";
    cout << "\n=======================================\n";
    cout << "Kode Pemesan : " << kodePemesan << "\n";
    cout << "Nama Penyewa : " << namaPemesan << "\n";
    cout << "---------------------------------------\n";
    cout << "Daftar Barang yang Disewa:\n";
    for (size_t i = 0; i < listBarang.size(); i++) {
        cout << "- " << listBarang[i].namaBarang << " (" << listBarang[i].merk << ")\n";
        cout << "  Jumlah : " << listJumlahSewa[i] << " unit\n";
        cout << "  Durasi : " << listDurasiHari[i] << " hari\n";
    }
    cout << "---------------------------------------\n";
    cout << "Tgl Pinjam   : " << tanggalPinjam << "\n";
    cout << "Tgl Kembali  : " << tanggalKembali << " (Estimasi Maks)\n";
    cout << "---------------------------------------\n";
    cout << "TOTAL HARGA  : Rp. " << fixed << setprecision(0) << totalHarga << "\n";
    cout << "=======================================\n";
}

void Transaksi::cetakNotaTxt() {
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
        file << "Daftar Barang:\n";
        for (size_t i = 0; i < listBarang.size(); i++) {
            file << "- " << listBarang[i].namaBarang << " " << listBarang[i].merk << "\n";
            file << "  Spesifikasi  : " << listBarang[i].spesifikasi << "\n";
            file << "  Jumlah       : " << listJumlahSewa[i] << " unit\n";
            file << "  Durasi       : " << listDurasiHari[i] << " hari\n";
        }
        file << "---------------------------------------\n";
        file << "Tgl Pinjam   : " << tanggalPinjam << "\n";
        file << "Tgl Kembali  : " << tanggalKembali << "\n";
        file << "---------------------------------------\n";
        file << "Total Bayar  : Rp. " << fixed << setprecision(0) << totalHarga << "\n";
        file << "Metode Bayar : " << metodeBayar << "\n";
        if (metodeBayar == "Cash" || metodeBayar == "CASH" || metodeBayar == "cash") {
            file << "Uang Dibayar : Rp. " << uangBayar << "\n";
            file << "Kembalian    : Rp. " << kembalian << "\n";
        }
        file << "=======================================\n";
        file << "      TERIMA KASIH TELAH MENYEWA       \n";
        file.close();
        cout << "[INFO] Nota berhasil dicetak (Nota_" << kodePemesan << ".txt)\n";
    }
}

// === IMPLEMENTASI CLASS ADMIN ===
bool Admin::login() {
    int sisaPercobaan = 3;
    while (sisaPercobaan > 0) {
        string inputPass = "";
        system("cls");
        cout << "=======================================\n";
        cout << "              LOGIN ADMIN              \n";
        cout << "=======================================\n";
        cout << "Masukkan Password Admin (Ketik 0 untuk Batal): ";

        char ch;
        while (true) {
            ch = _getch();
            if (ch == '\r' || ch == '\n') break;
            else if (ch == '\b') {
                if (!inputPass.empty()) {
                    inputPass.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                inputPass.push_back(ch);
                cout << '*';
            }
        }

        if (inputPass == "0") {
            cout << "\n\n[INFO] Login dibatalkan.\n";
            system("pause");
            return false;
        }
        if (inputPass == passwordMaster) return true;

        sisaPercobaan--;
        cout << "\n\n[ERROR] Password Salah!";
        if (sisaPercobaan > 0) {
            cout << " Sisa percobaan Anda: " << sisaPercobaan << "\n";
            system("pause");
        }
    }
    cout << "\n[AKSES DITOLAK] Anda salah memasukkan password sebanyak 3 kali.\n";
    system("pause");
    return false;
}

void Admin::lihatStok(const vector<Barang>& daftarBarang) {
    cout << "\n=== STOCK BARANG ===\n";
    for (const auto& b : daftarBarang) {
        cout << b.kodeBarang << " | " << b.namaBarang << " " << b.merk << " (" << b.spesifikasi << ") - Stok: " << b.stok << " - Rp." << b.hargaPerHari << "/hari\n";
    }
}

void Admin::restokBarang(vector<Barang>& daftarBarang) {
    string kode;
    int jml;
    lihatStok(daftarBarang);
    cout << "Masukkan Kode Barang yang ditambah/dikurangi stoknya: "; cin >> kode;
    jml = inputInt("Masukkan jumlah (gunakan minus untuk mengurangi): ");

    for (auto& b : daftarBarang) {
        if (b.kodeBarang == kode) {
            b.updateStok(jml);
            cout << "[SUKSES] Stok berhasil diupdate. Stok sekarang: " << b.stok << "\n"; return;
        }
    }
    cout << "[GAGAL] Kode barang tidak ditemukan.\n";
}

void Admin::tambahJenisBarang(vector<Barang>& daftarBarang) {
    string kb, nb, m, spek;
    double hrg;
    int stk;
    cout << "Kode Barang Baru: "; cin >> kb;
    cout << "Nama Barang: "; cin.ignore(); getline(cin, nb);
    cout << "Merk: "; getline(cin, m);
    cout << "Spesifikasi: "; getline(cin, spek);

    hrg = inputDouble("Harga / Hari: ");
    stk = inputInt("Stok Awal: ");

    daftarBarang.push_back(Barang(kb, nb, m, spek, hrg, stk));
    cout << "[SUKSES] Jenis barang baru berhasil ditambahkan.\n";
}

void Admin::monitoringPenyewa(vector<Transaksi>& daftarAktif, vector<Transaksi>& daftarSelesai, vector<Barang>& daftarBarang) {
    if (daftarAktif.empty()) { cout << "Tidak ada penyewa aktif.\n"; return; }
    cout << "\n=========================================\n";
    cout << "         DAFTAR PENYEWA AKTIF (BELUM KEMBALI) \n";
    cout << "=========================================\n";
    for (size_t i = 0; i < daftarAktif.size(); i++) {
        cout << i + 1 << ". Kode Sewa   : " << daftarAktif[i].kodePemesan << "\n";
        cout << "   Nama Penyewa: " << daftarAktif[i].namaPemesan << "\n";
        cout << "   No Telfon   : " << daftarAktif[i].noTelp << "\n";
        cout << "   Jaminan     : " << daftarAktif[i].jaminan << "\n";
        cout << "   Tgl Pinjam  : " << daftarAktif[i].tanggalPinjam << "\n";
        cout << "   Tgl Kembali : " << daftarAktif[i].tanggalKembali << "\n";
        cout << "   Total Biaya : Rp. " << fixed << setprecision(0) << daftarAktif[i].totalHarga << "\n";
        cout << "   Barang disewa:\n";
        for (size_t j = 0; j < daftarAktif[i].listBarang.size(); j++) {
            cout << "   -> " << daftarAktif[i].listBarang[j].namaBarang
                << " (" << daftarAktif[i].listJumlahSewa[j] << " unit) | " << daftarAktif[i].listDurasiHari[j] << " Hari\n";
        }
        cout << "-----------------------------------------\n";
    }

    int pil = inputInt("\nMenu Monitoring:\n1. Tandai Barang Kembali\n2. Kembali\nPilih: ");
    if (pil == 1) {
        int idx = inputInt("Pilih nomor urut penyewa yang mengembalikan barang: ");
        if (idx > 0 && idx <= (int)daftarAktif.size()) {
            size_t targetIdx = static_cast<size_t>(idx) - 1;

            for (size_t j = 0; j < daftarAktif[targetIdx].listBarang.size(); j++) {
                for (auto& b : daftarBarang) {
                    if (b.kodeBarang == daftarAktif[targetIdx].listBarang[j].kodeBarang) {
                        b.updateStok(daftarAktif[targetIdx].listJumlahSewa[j]);
                    }
                }
            }
            daftarSelesai.push_back(daftarAktif[targetIdx]);
            daftarAktif.erase(daftarAktif.begin() + targetIdx);
            cout << "[SUKSES] Semua barang dikembalikan. Stok bertambah dan masuk ke Log Sewa.\n";
        }
        else {
            cout << "[ERROR] Nomor urut tidak valid!\n";
        }
    }
}

void Admin::monitoringLogSewa(const vector<Transaksi>& daftarSelesai) {
    if (daftarSelesai.empty()) { cout << "Belum ada riwayat penyewaan selesai.\n"; return; }
    cout << "\n=========================================\n";
    cout << "         LOG PENYEWAAN SELESAI           \n";
    cout << "=========================================\n";
    for (const auto& log : daftarSelesai) {
        cout << "Kode: " << log.kodePemesan << " | Nama: " << log.namaPemesan << "\n";
        cout << "No Telp: " << log.noTelp << " | Jaminan: " << log.jaminan << "\n";
        cout << "Tgl Pinjam: " << log.tanggalPinjam << " | Tgl Kembali: " << log.tanggalKembali << " | Status: Selesai\n";
        for (size_t j = 0; j < log.listBarang.size(); j++) {
            cout << "  - " << log.listBarang[j].namaBarang << " (" << log.listJumlahSewa[j] << " unit)\n";
        }
        cout << "-----------------------------------------\n";
    }
}

// === IMPLEMENTASI CLASS SISTEM SEWA ===
string SistemSewa::generateKode(string prefix) {
    random_device rd; mt19937 gen(rd()); uniform_int_distribution<> dis(1000, 9999);
    return prefix + to_string(dis(gen));
}

void SistemSewa::inisialisasiDataAwal() {
    databaseBarang.push_back(Barang("S01", "Sepatu Gunung", "Eiger", "Ukuran 42", 25000, 10));
    databaseBarang.push_back(Barang("T01", "Tenda Dome", "Consina", "Kapasitas 4 Org", 50000, 5));
    databaseBarang.push_back(Barang("C01", "Carrier", "Arei", "60 Liter", 35000, 8));
}

void SistemSewa::lihatKatalog() {
    cout << "\n=======================================\n";
    cout << "            KATALOG ALAT GUNUNG           \n";
    cout << "=======================================\n";
    for (const auto& b : databaseBarang) {
        cout << "Kode  : " << b.kodeBarang << "\nNama  : " << b.namaBarang << " " << b.merk << "\nSpek  : " << b.spesifikasi
            << "\nHarga : Rp. " << b.hargaPerHari << " / hari\nStok  : " << b.stok << " unit\n---------------------------------------\n";
    }
}

void SistemSewa::fiturSewaBarang() {
    Transaksi trxBaru;
    cout << "\n=== INPUT DATA PENYEWA ===\n";
    trxBaru.kodePemesan = generateKode("TRX");
    cout << "Kode Pemesan\t: " << trxBaru.kodePemesan << "\n";
    cin.ignore();
    cout << "Nama Lengkap\t: "; getline(cin, trxBaru.namaPemesan);

    trxBaru.noTelp = inputNoTelp("No Telp(08xx)\t: ");
    cout << "Jaminan\t\t: "; getline(cin, trxBaru.jaminan);

    bool lanjutSewa = true;
    while (lanjutSewa) {
        system("cls");
        lihatKatalog();
        string kodePilih; cout << "Masukkan Kode Barang yang disewa: "; cin >> kodePilih;
        bool ketemu = false;
        Barang* barangTerpilih = nullptr;

        for (auto& b : databaseBarang) {
            if (b.kodeBarang == kodePilih) {
                barangTerpilih = &b;
                ketemu = true;
                break;
            }
        }
        if (!ketemu) {
            cout << "[ERROR] Kode barang tidak ditemukan. Ulangi.\n";
            system("pause");
            continue;
        }

        int jmlSewa = inputInt("Jumlah yang disewa: ");
        if (jmlSewa <= 0) {
            cout << "[ERROR] Jumlah sewa harus lebih dari 0!\n";
            system("pause");
            continue;
		}
        if (jmlSewa > barangTerpilih->stok) {
            cout << "[ERROR] Stok tidak mencukupi!\n";
            system("pause");
            continue;
        }

        int durasi = inputInt("Durasi Sewa (Hari): ");

        cout << "\nKonfirmasi: Anda memilih " << barangTerpilih->namaBarang << " sebanyak " << jmlSewa << " buah.\nApakah Anda yakin? (Y/N): ";
        char cYakin; cin >> cYakin;
        if (cYakin == 'Y' || cYakin == 'y') {
            barangTerpilih->updateStok(-jmlSewa);

            // FIX: Baris typo fatal sebelumnya sudah dihapus & diganti dengan push_back yang benar
            trxBaru.listBarang.push_back(*barangTerpilih);
            trxBaru.listJumlahSewa.push_back(jmlSewa);
            trxBaru.listDurasiHari.push_back(durasi);

            cout << "[SUKSES] Barang berhasil dimasukkan keranjang belanja.\n";
        }
        else {
            cout << "Batal menambahkan barang ini...\n";
        }

        cout << "\nApakah Anda ingin menyewa barang lain? (Y/N): ";
        char cLanjut; cin >> cLanjut;
        if (cLanjut == 'N' || cLanjut == 'n') {
            lanjutSewa = false;
        }
    }

    if (trxBaru.listBarang.empty()) {
        cout << "[INFO] Tidak ada barang yang disewa. Transaksi dibatalkan.\n";
        return;
    }

    system("cls");
    trxBaru.hitungWaktuOtomatis();
    trxBaru.hitungTotalBiaya();
    trxBaru.tampilkanRincian();

    cout << "Metode Pembayaran (Cash/TF): "; cin >> trxBaru.metodeBayar;
    if (trxBaru.metodeBayar == "Cash" || trxBaru.metodeBayar == "CASH" || trxBaru.metodeBayar == "cash") {
        do {
            trxBaru.uangBayar = inputDouble("Masukkan nominal uang bayar: Rp. ");
            if (trxBaru.uangBayar < trxBaru.totalHarga) cout << "[ERROR] Uang kurang!\n";
        } while (trxBaru.uangBayar < trxBaru.totalHarga);
        trxBaru.kembalian = trxBaru.uangBayar - trxBaru.totalHarga;
        cout << "Kembalian Anda: Rp. " << trxBaru.kembalian << "\n";
    }
    else {
        cout << "Silahkan Transfer ke Rekening BCA: 1234567890 a.n Rental Gunung\n";
        trxBaru.uangBayar = trxBaru.totalHarga; trxBaru.kembalian = 0;
    }
    trxBaru.cetakNotaTxt();
    transaksiAktif.push_back(trxBaru);
}
void SistemSewa::ubahdata() {
    char eh;
    bool lanjutSewa = true;
    while (lanjutSewa) {
        system("cls");
        lihatKatalog();
        cout << "[e] Edit [h] hapus [k] kembali: ";
        cin >> eh;
        
        if (eh == 'e' || eh == 'E') {
            string kodePilih; cout << "Masukkan Kode Barang yang ingin diubah: "; cin >> kodePilih;
            bool ketemu = false;
            Barang* barangTerpilih = nullptr;

            for (auto& b : databaseBarang) {
                if (b.kodeBarang == kodePilih) {
                    barangTerpilih = &b;
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) {
                cout << "[ERROR] Kode barang tidak ditemukan. Ulangi.\n";
                system("pause");
                continue;
            }
            if (ketemu == true) {
                string n ,m ,k ,s;
                double h;
                cin.ignore();
                cout << "ubah nama barang\t:";
                getline(cin, n);
                cout << "ubah merk barang\t:";
                getline(cin, m);
                cout << "ubah harga barang\t:";
                cin >> h;
                while (h < 0 || cin.fail()) {
                    cin.clear();
                    cin.ignore();
                    cout << "Tidak Valid!!\n";
                    cout << "ubah harga barang\t:";
                    cin >> h;
                }
                cin.ignore();
                cout << "ubah kode barang\t:";
                getline(cin, k);
                cout << "ubah spesifikasi barang\t:";
                getline(cin, s);

               
                (*barangTerpilih).ubahDetail(n, m, h, k, s);

                cout << "[SUKSES] Data barang berhasil diubah!\n";

            }

        }
        else if (eh == 'h' || eh == 'H') {

            string kodePilih;
            cout << "Masukkan Kode Barang yang ingin dihapus: ";
            cin >> kodePilih;

            bool ketemu = false;
            size_t indexHapus = 0;

            for (size_t i = 0; i < databaseBarang.size(); i++) {
                if (databaseBarang[i].kodeBarang == kodePilih) {
                    indexHapus = i;
                    ketemu = true;
                    break;
                }
            }

            if (!ketemu) {
                cout << "[ERROR] Kode barang tidak ditemukan.\n";
                system("pause");
                continue;
            }

            databaseBarang.erase(databaseBarang.begin() + indexHapus);

            cout << "[SUKSES] Barang berhasil dihapus.\n";
            system("pause");
        }
        else {
            return;
        }
    }

}   

void SistemSewa::menuAdmin() {
    if (!adminSistem.login()) {
        return;
    }
    int pil;
    do {
        system("cls");
        pil = inputInt("\n=== PANEL ADMIN ===\n1. Lihat Stok Barang\n2. Tambah/Kurangi Stok\n3. Tambah Jenis Barang Baru\n4. Monitoring Penyewa Aktif (Kembalikan Barang)\n5. Monitoring Log Sewa (Selesai)\n6. Ubah Data\n7. Keluar Panel Admin\nPilih: ");

        switch (pil) {
        case 1:
            system("cls"); adminSistem.lihatStok(databaseBarang);
            system("pause"); break;
        case 2:
            system("cls"); adminSistem.restokBarang(databaseBarang);
            system("pause"); break;
        case 3:
            system("cls"); adminSistem.tambahJenisBarang(databaseBarang);
            system("pause"); break;
        case 4:
            system("cls"); adminSistem.monitoringPenyewa(transaksiAktif, logSelesai, databaseBarang);
            system("pause"); break;
        case 5:
            system("cls"); adminSistem.monitoringLogSewa(logSelesai);
            system("pause"); break;
        case 7:
            cout << "Keluar dari menu admin...\n"; break;
        case 6:
            system("cls"); 
            ubahdata();
            system("pause"); break;
        default:
            cout << "Pilihan tidak valid!\n"; system("pause");
        }
    } while (pil != 7);
}

void SistemSewa::menuUtama() {
    int pilihan;
    do {
        system("cls");
       cout << "\n=======================================\n        SISTEM RENTAL ALAT GUNUNG       \n=======================================\n1. Sewa Barang\n2. Melihat Katalog Barang\n3. Admin\n4. Keluar\nPilihan Anda: ";
       cin >> pilihan;
        switch (pilihan) {
        case 1:
            system("cls"); fiturSewaBarang();
            system("pause"); break;
        case 2:
            system("cls"); lihatKatalog();
            system("pause"); break;
        case 3:
            menuAdmin(); break;
        case 4:
            cout << "Terima kasih telah menggunakan sistem ini!\n"; break;
        default:
            cin.clear();
            cin.ignore();
            cout << "Pilihan tidak valid, coba lagi.\n"; system("pause");
        }
    } while (pilihan != 4);
}