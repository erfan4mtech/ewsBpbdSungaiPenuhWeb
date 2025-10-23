// === URL API BMKG ===
const API_URL =
  "https://api.bmkg.go.id/publik/prakiraan-cuaca?adm4=15.72.03.2010";

const lokasiEl = document.getElementById("lokasi");
const cuacaContainer = document.getElementById("cuaca-container");

// === FETCH DATA BMKG ===
fetch(API_URL)
  .then((response) => {
    if (!response.ok) {
      throw new Error("Gagal memuat data dari BMKG");
    }
    return response.json();
  })
  .then((data) => {
    // === Ambil informasi lokasi ===
    const lokasi = data.lokasi;
    lokasiEl.innerHTML = `
      <h2>${lokasi.desa}, ${lokasi.kecamatan}</h2>
      <p>${lokasi.kotkab}, ${lokasi.provinsi}</p>
    `;

    // === Ambil daftar cuaca ===
    const cuacaList = data.data[0].cuaca.flat(); // gabungkan array harian jadi satu

    // === Buat tampilan kartu cuaca ===
    cuacaContainer.innerHTML = cuacaList
      .map(
        (item) => `
      <div class="card">
        <div class="time">${item.local_datetime
          .replace("2025-", "")
          .replace("T", " ")}</div>
        <img src="${item.image}" alt="${item.weather_desc}">
        <div class="desc">${item.weather_desc}</div>
        <div class="temp">${item.t}°C</div>
        <div class="hu">Kelembapan: ${item.hu}%</div>
      </div>
    `
      )
      .join("");
  })
  .catch((error) => {
    cuacaContainer.innerHTML = `<p style="color:red;">${error.message}</p>`;
  });
