const apiURL = "https://parkir.udnp4mtechno.com/api/getTinggiAir.php";

const ctx = document.getElementById("chartTinggiAir").getContext("2d");
let chart;

// elemen-elemen utama
const tbody = document.querySelector("#dataTable tbody");

// === Fungsi utama untuk ambil data dari API ===
function ambilData() {
  fetch(apiURL)
    .then((res) => {
      if (!res.ok) {
        throw new Error("HTTP " + res.status);
      }
      return res.json();
    })
    .then((json) => {
      if (json.kode === 1) {
        tampilkanData(json.data);
        tampilkanGrafik(json.data);
        console.log("Data berhasil dimuat.");
      } else {
        console.warn("Tidak ada data dari API.");
      }
    })
    .catch((err) => {
      console.error("Gagal memuat data:", err.message);
    });
}

// === Fungsi untuk menampilkan data ke tabel ===
function tampilkanData(data) {
  if (!tbody) return;
  tbody.innerHTML = "";

  data.forEach((d) => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${d.id}</td>
      <td>${d.tinggi_air}</td>
      <td>${d.lokasi_ews || "-"}</td>
      <td>${d.waktu}</td>
    `;
    tbody.appendChild(tr);
  });
}

// === Fungsi untuk menampilkan grafik ===
function tampilkanGrafik(data) {
  const labels = data.map((d) => d.waktu);
  const tinggiAir = data.map((d) => parseFloat(d.tinggi_air));

  if (chart) chart.destroy();

  chart = new Chart(ctx, {
    type: "line",
    data: {
      labels,
      datasets: [
        {
          label: "Tinggi Air (Meter)",
          data: tinggiAir,
          borderColor: "#2b7cff",
          backgroundColor: "rgba(43,124,255,0.15)",
          borderWidth: 2,
          pointRadius: 3,
          fill: true,
          tension: 0.3,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: { position: "top" },
        title: { display: true, text: "Grafik Tinggi Air Terbaru" },
      },
      scales: {
        x: { title: { display: true, text: "Waktu" } },
        y: { title: { display: true, text: "Tinggi Air (Meter)" } },
      },
    },
  });
}

// === Tombol manual refresh ===
const btnRefresh = document.getElementById("btnRefresh");
if (btnRefresh) {
  btnRefresh.addEventListener("click", ambilData);
}

// === Auto-refresh ===
const intervalSelect = document.getElementById("refreshInterval");
let intervalID;

if (intervalSelect) {
  intervalSelect.addEventListener("change", () => {
    clearInterval(intervalID);
    const val = parseInt(intervalSelect.value);
    if (val > 0) intervalID = setInterval(ambilData, val);
  });
}

// === Ambil data pertama kali ===
ambilData();
