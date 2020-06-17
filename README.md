# MultiCloud4u ARK IoT Presentation Code

[ARK.io](https://ark.io) IoT Blockchain Presentation Project Code created for [MultiCloud4u](https://multicloud4u.com/).

> [meetup.com/Delhi-Cloud-Computing-AI-and-Blockchain-Meetup-Group/events/271158348/](https://www.meetup.com/Delhi-Cloud-Computing-AI-and-Blockchain-Meetup-Group/events/271158348/)

## Objective

Use ARK to leverage the power of IoT using:
- VSCode
- PlatformIO
- ESP32 Development Board

## Steps

1) Get the Wallet Nonce.
2) Create the Transaction.
3) Broadcast the Transaction.
4) Search for the Transaction.
5) Blink LED when the Transaction is successfully forged on the ARK Developer Network.

## Example Output:
```shell
New Transaction:
{"version":2,"network":30,"typeGroup":1,"type":0,"nonce":"58","senderPublicKey":"0216fa03d378b6ad01325e186ad2cbb9d18976d5b27d0ca74b4f92bb6bf9a6d4d9","fee":"10000000","amount":"1","vendorField":"f7a67783096d350d7c92201d45845f59b7ff9ab5a69b91576def7c96afccf485","expiration":0,"recipientId":"DNSrsDUq5injGBdNXPV7v7u1Qy9LZfWEdM","id":"c60616ccce0278286bd45e12a6a19a1f178591a0661fa6b9e1dd975e06efa3bb","signature":"30440220505914c3713746dc36032cd730836128f9769568fbbab2ca5ce817e8b0788fab02207cfff90e481bf3929f5f7ac1cc29779de64d6381b18b6bf800260052f41d093e"}

broadcastResponse: {"data":{"accept":["c60616ccce0278286bd45e12a6a19a1f178591a0661fa6b9e1dd975e06efa3bb"],"broadcast":["c60616ccce0278286bd45e12a6a19a1f178591a0661fa6b9e1dd975e06efa3bb"],"excess":[],"invalid":[]}}

Searching For Tx..

Searching For Tx..

Searching For Tx..

Searching For Tx..

Searching For Tx..

Searching For Tx..

Searching For Tx..

TX was found!
```
