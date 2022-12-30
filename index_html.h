// HTML web page
const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=windows-1252" />
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style>
      body {
        font-family: expo-brand-demi, system-ui, -apple-system,
          BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial,
          "Noto Sans", sans-serif, "Apple Color Emoji", "Segoe UI Emoji",
          "Segoe UI Symbol", "Noto Color Emoji";
        background: #5e6d4e;
      }

      .circle {
        position: absolute;
        border-radius: 50%;
        background: white;
        animation: ripple 15s infinite;
        box-shadow: 0px 0px 1px 0px #508fb9;
      }

      .small {
        width: 20vh;
        height: 20vh;
        left: -10vh;
        bottom: -10vh;
      }

      .medium {
        width: 30vh;
        height: 30vh;
        left: -15vh;
        bottom: -15vh;
      }

      .large {
        width: 40vh;
        height: 40vh;
        left: -20vh;
        bottom: -20vh;
      }

      .xlarge {
        width: 50vh;
        height: 50vh;
        left: -25vh;
        bottom: -25vh;
      }

      .xxlarge {
        width: 60vh;
        height: 60vh;
        left: -30vh;
        bottom: -30vh;
      }

      .shade1 {
        opacity: 0.2;
        z-index: -1;
      }
      .shade2 {
        opacity: 0.3;
        z-index: -1;
      }

      .shade3 {
        opacity: 0.5;
        z-index: -1;
      }

      .shade4 {
        opacity: 0.7;
        z-index: -1;
      }

      .shade5 {
        opacity: 0.9;
        z-index: -1;
      }

      @keyframes ripple {
        0% {
          transform: scale(0.8);
        }

        50% {
          transform: scale(1.2);
        }

        100% {
          transform: scale(0.8);
        }
      }
      .wrapper {
        text-align: center;
        top: 0;
        bottom: 200px;
        left: 0;
        right: 0;
        margin: auto;
        height: 20px; /*requires explicit height*/
      }
      .formWrapper {
        margin-top: 37vh;
        flex: 1 1 auto;
        display: flex;
        flex-direction: column;
        padding: 40px;
        gap: 20px;
        align-items: center;
      }
      #input1 {
        width: 100%;
        z-index: 1;
      }
      #output1 {
        z-index: 1;
        color: black;
        font-size: x-large;
        background-color: lightblue;
        padding: 20px;
        width: 90px;
        border-radius: 50px; 
       
      }
      #buttonSubmit {
        align-items: center;
        background-color: initial;
        background-image: linear-gradient(#464d55, #25292e);
        border-radius: 8px;
        border-width: 0;
        box-shadow: 0 10px 20px rgba(0, 0, 0, 0.1),
          0 3px 6px rgba(0, 0, 0, 0.05);
        box-sizing: border-box;
        color: #fff;
        cursor: pointer;
        display: inline-flex;
        flex-direction: column;
        font-size: 18px;
        height: 52px;
        justify-content: center;
        line-height: 1;
        margin: 0;
        outline: none;
        overflow: hidden;
        padding: 0 32px;
        text-align: center;
        text-decoration: none;
        transform: translate3d(0, 0, 0);
        transition: all 150ms;
        vertical-align: baseline;
        white-space: nowrap;
        user-select: none;
        -webkit-user-select: none;
        touch-action: manipulation;
      }

      @media (max-width: 420px) {
        .button-84 {
          height: 48px;
        }
      }     
    </style>
  </head>
  <body>
    <div class="ripple-background">
      <div class="circle xxlarge shade1"></div>
      <div class="circle xlarge shade2"></div>
      <div class="circle large shade3"></div>
      <div class="circle mediun shade4"></div>
      <div class="circle small shade5"></div>
    </div>
    <div class="wrapper">
     <canvas id="loadingCvs"></canvas>
      <form action="/get">
        <div class="formWrapper">
          <input
            id="input1"
            type="range"
            value="2"
            min="1"
            max="300"
            name="input1"
            oninput="this.nextElementSibling.value = this.value"
          />
          <output id="output1">2</output>
          <div id="buttonSubmitWrapper">
            <input id="buttonSubmit" type="submit" value="Submit" />
          </div>
          </div>
      </form>
    </div>
    <br />
    <script>
      document.getElementById("input1").defaultValue = %DATA%.winderRounds;
      document.getElementById("output1").innerHTML = %DATA%.winderRounds;
      document.getElementById("loadingCvs").style.display = "none"

      var source = new EventSource('/events');
      var percent = 60;
     
      source.addEventListener('loadingState', function(e) {
      
      let hasData = e.data > 0;
      
      document.getElementById("input1").style.display = hasData ? "none":"block";
      document.getElementById("output1").style.display = hasData ? "none":"block";
      document.getElementById("buttonSubmitWrapper").style.display = hasData ? "none":"block";

      var canvas = document.getElementById("loadingCvs");
      canvas.style.display = hasData ? "block":"none";

      if(hasData){
        /* -8 avoid canvas moving */
        canvas.width = (window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth) -8;
        canvas.height = (window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight) -8;

        percent = e.data;        
        var ctx = canvas.getContext("2d");

        ctx.beginPath();
        ctx.arc(innerWidth / 2, innerHeight / 2, 100, 0, Math.PI * 2);
        ctx.strokeStyle = "#262626";
        ctx.lineWidth = 20;
        ctx.stroke();
        ctx.closePath();

        var angle = (percent / 100) * 360;
        ctx.beginPath();
        ctx.arc(
          innerWidth / 2,
          innerHeight / 2,
          100,
          (-90 * Math.PI) / 180,
          ((angle - 90) * Math.PI) / 180
      );
      ctx.strokeStyle = "lightblue";
      ctx.lineWidth = 20;
      ctx.stroke();
      ctx.closePath();

      ctx.textBaseline = "middle";
      ctx.textAlign = "center";
      ctx.font = "40px arial bold";
      ctx.fillStyle = "lightblue";
      ctx.fillText(percent + "%", innerWidth / 2, innerHeight / 2);
      }
      
      }, false);
     
      

      function toggleCheckbox(x) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/" + x, false);
        xhr.send();
        setTimeout(() => {
          window.history.go(-1);
        }, 1000);
      }
    </script>
  </body>
</html>


)rawliteral";
