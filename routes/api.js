/**
  Api.js is used for all calls to the WeatherStation
  Author: Isaac Assegai
*/

var express = require('express');
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
  res.send('ZipCode: ' + zipCode);
});

module.exports = router;
