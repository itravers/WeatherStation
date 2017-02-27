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
router.get('/oneDay/:zipCode/:boxVersion/:serialNum', function(req, res){
  var zipCode = req.params.zipCode;

  

  //first check if the db already has info for given zipCode within the last hour
  var db = req.db;
  var collection = db.get('weatherqueries');

  //find all items created within last hour, with the given zip
  var dateNow = Math.floor(new Date() / 1000);
  var dateHourAgo = Math.floor(new Date() / 1000) - (60*60*1); //get unix timestamp 1 hour ago
  var query = {"time": {$gt:dateHourAgo}, "zipcode":zipCode};
  collection.find(query, function(e, docs){
    console.log("dateHourAgo: " +dateHourAgo + "  dateNow: " + dateNow+ " dateDifference: " +(dateNow - dateHourAgo)+ " docs: " + docs + " e: " + e);

  //check if a record was returned matching the zip, that was written within the last hour
  if(docs.length > 0){
    //a record was returned, we want to send this data back to the caller, formatted for the hardware
    console.log("forcast: " + docs[0]["forecast"]);
    res.send(docs[0]["forecast"]); //send the forecast to the hardware
  }else{
    /* a record was not returned, we need to make an api request to get the info save it to the db
       and send it back to the caller, formatted for the hardware. */

    //Real API REQUEST
    request('http://api.wunderground.com/api/411734c88ec29497/forecast/q/'+zipCode+'.json', function(error, response, body){ 
      if(!error && response.statusCode == 200){
        var jsonData = JSON.parse(body);
        if(jsonData && jsonData.forecast){
          var lowTemp = jsonData.forecast.simpleforecast.forecastday[1].low.fahrenheit;
          var highTemp = jsonData.forecast.simpleforecast.forecastday[1].high.fahrenheit;
          var precipChance = jsonData.forecast.simpleforecast.forecastday[1].pop;
      
          console.log("$" + lowTemp + "$" + highTemp + "$" + precipChance);

          //save the data to the db
          var ip = req.connection.remoteAddress;
          var time = dateNow;
          var forecast = "$" + lowTemp + "$" + highTemp + "$" + precipChance;
          var boxVersion = req.params.boxVersion;
          var serialNum = req.params.serialNum;
          // Submit to the DB
          collection.insert({
              "ip" : ip,
              "zipcode" : zipCode,
              "time"   : time,
              "forecast"   : forecast,
              "boxversion"  : boxVersion,
              "serialNum"  : serialNum
          }, function (err, doc) {
              if (err) {
                  // If it failed, return error
                  res.send("There was a problem adding the information to the database.");
              }
              else {
                  //send the data to the hardward box
                  res.send("$" + lowTemp + "$" + highTemp + "$" + precipChance);
              }
          });

        }else{
          console.log("Json Data for: "+ zipCode + " is undefined");
        }
      }else{
        res.send("There was an error: " + response.statusCode + " : " + error);
      }
    }); 
  }
});

/*
//Real API REQUEST
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
*/ //end real api request
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
