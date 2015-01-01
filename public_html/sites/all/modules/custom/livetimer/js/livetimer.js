Drupal.behaviors.livetimer = {
  attach: function(context) {
    livetimer();
  }
};

/**
 * Date diff in seconds.
 */
function DateDiff(date1,date2) {
  return (date1.getTime() - date2.getTime())/1000; // in seconds
}

/**
 * Display time.
 */
function livetimer(currenttime){
  var starttime = parseDateTimer(Drupal.settings.livetimer.starttime);
  var classname = Drupal.settings.livetimer.classname;
  if (currenttime == null) {
    currenttime = parseDateTimer(Drupal.settings.livetimer.currenttime);
  }
  else {
    currenttime.setSeconds(currenttime.getSeconds() + 1);
  }
  var interval = DateDiff(currenttime, starttime);
  var hours = parseInt( interval / 3600 ) % 24;
  var minutes = parseInt( interval / 60 ) % 60;
  var seconds = interval % 60;

  var result = (hours < 10 ? "0" + hours : hours) + ":" + (minutes < 10 ? "0" + minutes : minutes) + ":" + (seconds  < 10 ? "0" + seconds : seconds);

  jQuery('.' + classname).html(result);
  jQuery('.' + classname).timerID = setTimeout(function(){
    livetimer(currenttime);
  },1000);
};

/**
 * Fix IE8 error with Date.parse method.
 */
function parseDateTimer(input) {
  var parts = input.match(/(\d+)/g);
  return new Date(parts[0], parts[1]-1, parts[2], parts[3], parts[4], parts[5]); // months are 0-based
}