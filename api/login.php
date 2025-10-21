<?php


// Izinkan semua origin
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST, GET, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type");

// Tangani preflight request
if ($_SERVER['REQUEST_METHOD'] == 'OPTIONS') {
    http_response_code(200);
    exit();
}

require("koneksi.php");

$response = array();

if($_SERVER['REQUEST_METHOD'] == 'POST'){

    $uName = $_POST["uName"];
    $uPass = $_POST["uPass"];

    // persiapkan query dengan placeholder
    $perintah = "SELECT * FROM users WHERE uName = ? AND uPass = ?";

    //Persiapkan statement
    if($stmt = mysqli_prepare($konek, $perintah)){
        mysqli_stmt_bind_param($stmt,"ss", $uName, $uPass);

        mysqli_stmt_execute($stmt);

        $result = mysqli_stmt_get_result($stmt);

        if(mysqli_num_rows($result) > 0){
            $response["kode"] = 1;
            $response["pesan"] = "Data Di Temukan";
            $response["data"] = array();

            //ambil data dari hasil query
            while($ambil = mysqli_fetch_object($result)){
                 $F["id"] = $ambil -> id;
                 $F["uName"] = $ambil -> uName;
                 $F["nama"] = $ambil -> nama;
                 $F["nik"] = $ambil -> nik;
                 $F["tingkatan"] = $ambil -> tingkatan;
                 $F["patch"] = $ambil -> patch;
                 $F["tanggal_bergabung"] = $ambil -> tanggal_bergabung;

                 array_push($response["data"], $F);
            }
           

        }else{
            $response["kode"] = 0;
            $response["pesan"] = "Data Tidak Ditemukan";
        }

    }else{
        $response["kode"] = 0;
        $response["pesan"] = "Gagal Menyiapkan Query";
    }


}else{
    $response["kode"] = 0;
    $response["pesan"] = "Tidak Ada Post Data";

}
echo json_encode($response);
mysqli_close($konek);