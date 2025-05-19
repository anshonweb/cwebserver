function FetchCatch() {
  const catImageUrl = "http://localhost:8080/cat.png";
  document.getElementById("catImage").src = catImageUrl; // prevent caching
}

document.addEventListener("DOMContentLoaded", function() {
  document.getElementById("catBtn").addEventListener("click", FetchCatch);
});



document.getElementById("fetch404").addEventListener("click", () => {
  fetch("http://localhost:8080/abcd")
    .then(async (response) => {
      const text = await response.text();
      if (!response.ok) {
       
        document.getElementById("output").textContent = "Server responded with:\n" + text;
      } else {
        document.getElementById("output").textContent = text;
      }
    })
    .catch((error) => {
      document.getElementById("output").textContent = "Fetch error: " + error;
    });
});
