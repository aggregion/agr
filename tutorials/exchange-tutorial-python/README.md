The following steps must be taken for the example script to work.

0. Create wallet
0. Create account for agrio.token
0. Create account for scott
0. Create account for exchange
0. Set token contract on agrio.token
0. Create AGR token
0. Issue initial tokens to scott

**Note**:
Deleting the `transactions.txt` file will prevent replay from working.


### Create wallet
`clagr wallet create`

### Create account steps
`clagr create key`

`clagr create key`

`clagr wallet import  --private-key <private key from step 1>`

`clagr wallet import  --private-key <private key from step 2>`

`clagr create account agrio <account_name> <public key from step 1> <public key from step 2>`

### Set contract steps
`clagr set contract agrio.token /contracts/agrio.token -p agrio.token@active`

### Create AGR token steps
`clagr push action agrio.token create '{"issuer": "agrio.token", "maximum_supply": "100000.0000 AGR", "can_freeze": 1, "can_recall": 1, "can_whitelist": 1}' -p agrio.token@active`

### Issue token steps
`clagr push action agrio.token issue '{"to": "scott", "quantity": "900.0000 AGR", "memo": "testing"}' -p agrio.token@active`
