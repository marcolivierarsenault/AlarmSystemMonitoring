console.log('Loading function');

var AWS = require('aws-sdk');  
AWS.config.region = 'us-east-1';

exports.handler = (event, context, callback) => {
        var sns = new AWS.SNS();
    sns.publish({
        Message: 'THERE IS AN ALARM',
        Subject: 'System d\'alarme Maison',
        TopicArn: 'arn:aws:sns:us-east-1:209788288140:sendEmailAndSms'
    }, function(err, data) {
        if (err) {
            console.log(err.stack);
            return;
        }
        console.log('push sent');
        console.log(data);
        context.done(null, 'Function Finished!');  
    });
    
    callback(null, 'Alarm Sent');
};
