// ESP32 එකෙන් දැනට Save වී ඇති කාලයන් ලබාගෙන Input boxes වලට දමයි
function getTimes() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            document.getElementById("onTime").value = data.on;
            document.getElementById("offTime").value = data.off;
        }
    };
    xhr.open("GET", "/getTimes", true);
    xhr.send();
}

// අලුත් කාලයන් Save කිරීමට server එකට යැවීම
function saveTimer() {
    var onTime = document.getElementById("onTime").value;
    var offTime = document.getElementById("offTime").value;
    var statusText = document.getElementById("status");
    
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            statusText.innerHTML = "කාලයන් සාර්ථකව Save විය!";
            statusText.className = "on";
            setTimeout(() => { statusText.innerHTML = "ස්වයංක්‍රීයව ක්‍රියාත්මක වේ"; }, 2000);
        }
    };
    
    xhr.open("GET", "/setTimer?on=" + onTime + "&off=" + offTime, true);
    xhr.send();
}