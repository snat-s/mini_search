const input = document.getElementById("linkInput");
const submitButton = document.getElementById("submitButton");
const response = document.getElementById("response");

submitButton.addEventListener("click", () => {
    const query = input.value;
    input.value = "";
    response.innerHTML = "Loading...";
    /*
      Make a loading for the input.
     */
    fetch("http://localhost:8080/", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: query
    })
        .then(res => res.json())
        .then(data => {
            response.innerHTML = data.routes;
        })
        .catch(error => {
            response.innerHTML = "The query could not be served.";
            console.error(error);
        });
});
