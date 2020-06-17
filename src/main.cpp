/*******************************************************************************
 * Copyright (c) ARK Ecosystem <info@ark.io>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "connection/connection.h"  // <- from C++ Client

// C++ Crypto Headers
#include "crypto/hash.hpp"
#include "transactions/builders/builder.hpp"
#include "utils/hex.hpp"
#include "utils/json.h"

using namespace Ark::Client;
using namespace Ark::Crypto;
using namespace Ark::Crypto::transactions;

// const char* ssid = "yourSDID";
// const char* password = "yourPassword";
#include "secret.h"

////////////////////////////////////////////////////////////////////////////////
// Globals
Connection<Api> client("167.114.29.52", 4003);
char chipHash[HASH_64_LEN + 1] = {'0'};

////////////////////////////////////////////////////////////////////////////////
// Helper Functions
void createHash(char hashBuffer[64]) {
  uint64_t chipId = ESP.getEfuseMac();
  uint8_t byteArray[8];
  memmove(byteArray, &chipId, sizeof(uint64_t));
  const auto shaHash = Hash::sha256(&byteArray[0], sizeof(uint64_t));
  memmove(hashBuffer,
          BytesToHex(&shaHash[0], &shaHash[0] + HASH_32_LEN).c_str(),
          HASH_64_LEN);
}

void blink() {
  auto blinkCount = 0;
  do {
    digitalWrite(13, HIGH);
    delay(1000);

    digitalWrite(13, LOW);
    delay(1000);

    ++blinkCount;
  } while (blinkCount < 10);

  esp_deep_sleep_start();
}

////////////////////////////////////////////////////////////////////////////////
// Methods using the ARK C++ Client API

uint64_t getWalletNonce(Connection<Api>& client, const char* address) {
  const auto walletResponse = client.api.wallets.get(address);

  const size_t capacity =
      JSON_OBJECT_SIZE(0) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(6) + 160;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, walletResponse);
  uint64_t nonce = doc["data"]["nonce"];

  return nonce + 1;
}

// Create a Transaction using the Wallet Nonce and Board ChipId Hash.
Transaction createTransaction() {
  const auto nonce = getWalletNonce(client, RECIPIENT_ID);
  createHash(chipHash);

  const auto transaction = builder::Transfer()
                               .nonce(nonce)
                               .amount(1)
                               .expiration(0)
                               .recipientId(RECIPIENT_ID)
                               .vendorField(chipHash)
                               .sign(PASSPHRASE)
                               .build();

  Serial.printf("\n\nNew Transaction:\n%s\n\n", transaction.toJson().c_str());

  return transaction;
}

// Broadcast a Transaction using the 'createTransaction()' method.
void sendTransaction() {
  const auto transaction = createTransaction();
  auto txPayload = "{\"transactions\":[" + transaction.toJson() + "]}";

  const auto broadcastResponse = client.api.transactions.send(txPayload);
  Serial.printf("\nbroadcastResponse: %s\n", broadcastResponse.c_str());
}

// Search for the broadcast Transaction using the ChipId Hash.
bool txWasFound() {
  const auto txSearchResponse =
      client.api.wallets.transactionsReceived(RECIPIENT_ID, "?page=1&limit=3");

  return txSearchResponse.find(chipHash) < txSearchResponse.length();
}

////////////////////////////////////////////////////////////////////////////////
// Sketch Methods
void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

//   createTransaction();
  sendTransaction();

  pinMode(13, OUTPUT);
}

void loop() {
  if (txWasFound()) {
    Serial.println("\nTX was found!");
    blink();
  } else {
    Serial.println("\nSearching For Tx..");
  };
}
