# Porcoduino
Porcoduino is an experimentation to craft a Zeppelin in Arduino.

## Electronics parts
You needs electronics parts:

* 1 L293D
* 2 Xbee Module
* 1 Arduino
* 2 DC Motors
* 1 XBee shield (for Arduino)
* 1 XBee shield (to connect on your computer)

## Schema
This schema show how to put together electonics parts for zeppelin:
![Porcoduino Fritzing schema](https://raw.github.com/fbentz/Porcoduino/master/docs/Porcoduino_bb.png)

## App
App will manage your zeppelin.

You need to install nodejs v.0.10.32 and install dependencies

```
  npm install
```

You need to set required information on config.json

```
{
  "xbee": {
    "serialPort":"/dev/tty.usbserial-A601EZ5O",
    "baudrate": 9600
  },
  "server": {
    "port": 8000
  }
}

```
and run app with

```
  npm start
```

if you want test your environnement run

```
  npm test
```
## Contributors
* [Fabrice Bentz](https://github.com/fbentz/)
* [Robin Passama](https://github.com/passama/)
