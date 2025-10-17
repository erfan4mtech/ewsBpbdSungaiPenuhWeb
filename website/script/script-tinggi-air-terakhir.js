document.addEventListener("DOMContentLoaded", () => {
  const tinggiAirTerakhir = document.getElementById("ketinggian-air");
  const statusTinggiAir = document.getElementById("status-tinggi-air");
  const apiURL = "https://parkir.udnp4mtechno.com/api/getTinggiAirTerakhir.php";

  function ambilData() {
    fetch(apiURL)
      .then((response) => {
        if (!response.ok) {
          throw new Error("Gagal Mengambil Data Dari Server: " + response.status);
        }
        return response.json();
      })
      .then((json) => {
        console.log("Data diterima:", json);
        if (json.kode === 1 && json.data && json.data.length > 0) {
          const dataTerakhir = json.data[0];
          const tinggi = parseFloat(dataTerakhir.tinggi_air).toFixed(2);
          const status = dataTerakhir.status?.trim();

          tinggiAirTerakhir.textContent = `${tinggi} Meter`;
          statusTinggiAir.textContent = status;

          const warnaStatus = {
            AMAN: "green",
            SIAGA: "gold",
            WASPADA: "orange",
            AWAS: "red",
          };

          // Terapkan warna & gaya status
          statusTinggiAir.style.color = warnaStatus[status] || "black";
          statusTinggiAir.style.fontWeight = "bold";
          statusTinggiAir.style.transition = "color 0.5s ease";

          // 🔥 Jika status "AWAS", aktifkan blink
          if (status === "AWAS") {
            statusTinggiAir.classList.add("blink");
          } else {
            statusTinggiAir.classList.remove("blink");
          }

        } else {
          tinggiAirTerakhir.textContent = "Data Tidak Tersedia";
          statusTinggiAir.textContent = "Data Tidak Tersedia";
          statusTinggiAir.style.color = "black";
          statusTinggiAir.classList.remove("blink");
        }
      })
      .catch((error) => {
        console.error("Error:", error);
        tinggiAirTerakhir.textContent = "Gagal memuat data";
        statusTinggiAir.textContent = "-";
        statusTinggiAir.style.color = "black";
        statusTinggiAir.classList.remove("blink");
      });
  }

  // Panggil pertama kali
  ambilData();

  // Tombol manual refresh
  const btnRefresh = document.getElementById("btnRefresh");
  if (btnRefresh) btnRefresh.addEventListener("click", ambilData);

  // Auto-refresh
  const intervalSelect = document.getElementById("refreshInterval");
  let intervalId;

  if (intervalSelect) {
    intervalSelect.addEventListener("change", () => {
      if (intervalId) clearInterval(intervalId);
      const val = parseFloat(intervalSelect.value);
      if (val > 0) {
        intervalId = setInterval(ambilData, val);
        console.log(`Auto-refresh aktif setiap ${val / 1000} detik`);
      } else {
        console.log("Auto-refresh dimatikan");
      }
    });
  }
});