#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>

struct Package {
    int receipt_id;             
    int hub_id;                
    std::string kurir_id;       
    std::string current_hub;    
    std::string hub_asal;       
    std::string hub_akhir;     
    std::string jenis_barang;   
    std::string alamat_tujuan;  
};

#endif