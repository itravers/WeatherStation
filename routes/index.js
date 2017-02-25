var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/uses', function(req, res){
  var db = req.db;
  var collection = db.get('weatherqueries');
  collection.find({}, {}, function(e, docs){
    res.render('uses', {
      moment: require('moment'),
      "uses" : docs
    });
  });
});

module.exports = router;
