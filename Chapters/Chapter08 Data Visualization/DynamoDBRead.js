      // Load the AWS SDK for Node.js
      var AWS = require('aws-sdk');
      
      // Set the region 
      AWS.config.update({region: 'us-west-2'});
      
      // Create the DynamoDB service object
      var ddb = new AWS.DynamoDB();
      
      var params = {
        TableName: 'IoTCentral',
        Key: {
          'Device ID': {S: '866425031237797'},
          'Timestamp': {S: '84810'}
        }
      };
      
      // Call DynamoDB to read the item from the table
      ddb.getItem(params, function(err, data) {
        if (err) {
          console.log("Error", err);
        } else {
          data = JSON.stringify(data.Item);
          dataParse = JSON.parse(data)               
          IMEI = dataParse["Device ID"];
          Timestamp = dataParse["Timestamp"];               
          dataPayloadParse = dataParse["payload"].M
          console.log("DeviceID: ",IMEI.S);
          console.log("Timestamp: ",Timestamp.S);
          console.log("payload: ",dataPayloadParse);                
  }
      });