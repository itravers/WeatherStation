/**
  Api.js is used for all calls to the WeatherStation
  Author: Isaac Assegai
*/

var express = require('express');
var request = require('request');
var fs = require('fs'); //used to open jsonTest.json
var testJson = require('../jsonTest.json'); //used to test json formatting without making api calls

var router = express.Router();

/* GET users listing. */
router.get('/', function(req, res, next) {
  res.send('this is the api page');
});

/* The 3 day api
   Accepts a zip code
   Returns 3 Numbers
   First: Low Temp for Tomorrow in F.
   Second: High Temp fot Tomorrow in F.
   Third: Chance of Precipitation Tomorrow in %.
*/
router.get('/threeDay/:zipCode', function(req, res){
  var zipCode = req.params.zipCode;
  
  //make the api request
/* THIS IS THE REAL API REQUEST
  request('http://api.wunderground.com/api/411734c88ec29497/forecast/q/'+zipCode+'.json', function(error, response, body){
    if(!error && response.statusCode == 200){
      res.send(body);
    }else{
      res.send("There was an error: " + response.statusCode + " : " + error);
    }
  });
*/ //END REAL API REQUEST

//FAKE API REQUEST
  //request('http://localhost:3001/api/testJson', function(error, response, body){
  request('http://api.wunderground.com/api/411734c88ec29497/forecast/q/'+zipCode+'.json', function(error, response, body){ 
   if(!error && response.statusCode == 200){
      var jsonData = JSON.parse(body);
      if(jsonData && jsonData.forecast){
      	var lowTemp = jsonData.forecast.simpleforecast.forecastday[1].low.fahrenheit;
      	var highTemp = jsonData.forecast.simpleforecast.forecastday[1].high.fahrenheit;
      	var precipChance = jsonData.forecast.simpleforecast.forecastday[1].pop;
      
        console.log("$" + lowTemp + "$" + highTemp + "$" + precipChance);
        res.send("$" + lowTemp + "$" + highTemp + "$" + precipChance);
      }else{
        console.log("Json Data for: "+ zipCode + " is undefined");
      }
    }else{
      res.send("There was an error: " + response.statusCode + " : " + error);
    }
  }); 

});

//test json
router.get('/testJson', function(req, res){
  //var json = JSON.parse(fs.readFileSync('jsonTest.json', 'utf8'));
  var file = fs.readFileSync('jsonTest.json', 'utf8');

  res.send(file);
});

//connection test for arduino firmware
router.get('/connectionTest', function(req, res){
  res.send("Connected to API");

});


module.exports = router;
