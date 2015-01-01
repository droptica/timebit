Drupal.behaviors.liveclock = {
  attach: function(context) {
    live_clock();
   }
};  
  
/**
 * Display time.
 */
function live_clock(time){
  if (time == null) {    
    time = parseDate(Drupal.settings.liveclock.time);
  }
  else {
    time.setSeconds(time.getSeconds() + 1);
  }

  var year = time.getFullYear();
  var month = time.getMonth() +1;
  var day = time.getDate();
  var hours = time.getHours();
  var minutes = time.getMinutes();
  var seconds = time.getSeconds();

  if (minutes<10) {
    minutes = '0' + minutes;
  }
  if (seconds<10) {
    seconds = '0' + seconds;
  }
  if (month<10) {
    month = '0' + month;
  }
  if (day<10) {
    day = '0' + day;
  }
  
  jQuery('.live-server-clock').html(year +"-"+ month + '-' + day + " " + hours + ":" + minutes + ":" + seconds);
  jQuery('.live-server-clock').timerID = setTimeout(function(){
    live_clock(time);
  },1000);


};

/**
 * Fix IE8 error with Date.parse method.
 */
function parseDate(input) {
  var parts = input.match(/(\d+)/g);  
  return new Date(parts[0], parts[1]-1, parts[2], parts[3], parts[4], parts[5]); // months are 0-based
}