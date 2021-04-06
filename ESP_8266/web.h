const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html> 
<html>
<meta charset = "utf-8"> 
<title> 
Demo Web
</title> 
<style> 
.g-block-1 {
    width: 50%;
    float: left;
}
.g-block-2 {
    width: 50%;
    float: right;
}
.block1 {
    width: 400px;
    height: 100px;
    background-color: yellow;
  border-top-left-radius: 100px 100px;
  border-bottom-right-radius: 100px 100px;
}
.block2 {
    width: 400px;
    height: 100px;
    background-color: cyan;
  border-top-left-radius: 100px 100px;
  border-bottom-right-radius: 100px 100px;
}
.block3 {
    width: 500px;
    height: 250px;
  border-radius: 100px;
    background-color: DeepSkyBlue;
}


</style> 
<body> 
<center>
<h1> Demo hệ thống theo dõi</h1> 
<h2> Web Design By Tran Thanh </h2> 
</center> 
<div>
  <div class="g-block-1"> 
    <center> 
      <div class="block1">
        <h1> Nhiệt độ </h1>
        <label id = 'nhietdo'> 0 </label>  
      </div> 
      <div class = "block2"> 
        <h1> Độ ẩm </h1> 
        <label id = 'doam'> 0 </label> 
      </div>
    </center> 
  </div>
  <div class="g-block-2"> 
    <center> 
      <div class = "block3"> 
        <h1> Data nhận được</h1> 
        <label id = 'dataesp'> 0 </label>
      </div> 
    </center> 
  </div>

</div>
</body> 
<script> 
window.onload = UpdataData;
function UpdataData()
{
  var xhttp = new XMLHttpRequest(); 
  xhttp.onreadystatechange = function ()
  {
    if(this.readyState == 4 && this.status == 200)
    { 
      var DataVDK = xhttp.responseText; 
      console.log("Dữ liệu từ ESP: " + DataVDK);
      var DataJson = JSON.parse(DataVDK); 
      document.getElementById("dataesp").innerHTML = DataVDK;
      document.getElementById("nhietdo").innerHTML = DataJson.nhietdo;
      document.getElementById("doam").innerHTML = DataJson.doam; 
      
    }
  }
  xhttp.open('GET','/push_data', true);
  xhttp.send(); 
  setTimeout(function(){UpdataData()}, 500);
}
</script> 
</html> 
)=====";
