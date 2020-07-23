      // Load the AWS SDK for Node.js
      var AWS = require('aws-sdk');
      
      // Set the region 
      AWS.config.update({region: 'us-west-2'});
      
      // Create the DynamoDB service object
      var ddb = new AWS.DynamoDB();

      var params = {
        TableName: "IoTCentral"
      };      
      
      // Call DynamoDB to read the item from the table
      var count = 1;
      var tempArray=[];
      ddb.scan(params, function(err, data) {
        if (err) {
          console.log("Error", err);
        } else {
            data.Items.forEach(function(itemdata) 
            {
                console.log("Record :", count);
                data = JSON.stringify(itemdata);
                dataParse = JSON.parse(data)               
                IMEI = dataParse["Device ID"];
                Timestamp = dataParse["Timestamp"];               
                dataPayloadParse = dataParse["payload"].M
                Temperature = dataPayloadParse["Temperature"].S;
                tempArray[count] = Temperature;
                console.log("IMEI: ",IMEI.S);
                console.log("Timestamp: ",Timestamp.S); 
                console.log("payload: ",dataPayloadParse);     
                console.log("Temperature: ",Temperature);
                console.log("All Temperature: ",tempArray.values());                
                for (let elements of tempArray.values() ) { 
                  console.log(elements); 
                }                 
                count++;
             });            
        }
      });