document.addEventListener("DOMContentLoaded", () => {
    const btnLogin = document.getElementById("btn-login");
    const intputUser = document.getElementById("user_name");
    const intputPass = document.getElementById("user_pass");

    const apiLogin = "https://parkir.udnp4mtechno.com/api/logingBpbd.php"

    btnLogin.addEventListener("click", async () =>{
        const user_name = intputUser.value.trim();
        const user_pass = intputPass.value.trim();

        if(!user_name || !user_pass){
            alert("Username dan Password wajib diisi!");
            return;
        }

        // Siapkan data POST
        const formData = new FormData();
        formData.append("user_name", user_name);
        formData.append("user_pass", user_pass);

        try{
            const response = await fetch(apiLogin, {
                method: "POST",
                body: formData
            });

            const data = await response.json();

            if(data.kode === 1){
                //login berhasil
                const namaLengkap = data.data[0].nama_lengkap;
                

                //simpan nama_lengkap di localStorage agar visa ditampilkan di ews-banjir.html

                localStorage.setItem("nama_lengkap", namaLengkap);

                // redirect ke halaman ews-banjir.html
                window.location.href = "ews-banjir.html";
            }else{
                alert(data.pesan || "Login Gaggal");
            }
        }catch(error){
                console.error("Error login: ", error);
                alert("Terjadi Kesalahan Saat Login!")
        }
    });
});