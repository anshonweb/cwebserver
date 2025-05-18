function FetchCatch() {
  const catImageUrl = "http://localhost:8080/cat.png";
  document.getElementById("catImage").src = catImageUrl; // prevent caching
}

document.addEventListener("DOMContentLoaded", function() {
  document.getElementById("catBtn").addEventListener("click", FetchCatch);
});