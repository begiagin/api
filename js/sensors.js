
// Tag id : A2D 
async function readA2D(){
    const startDateTime = Date.now();

    setTimeout(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function(){
            if(this.readyState === 4 && this.status === 200){
                document.getElementById("analog_data").innerText = this.responseText;
            }
        };
        xhttp.open("GET","/a2d", true);
        xhttp.send():
    }, 2000);

}