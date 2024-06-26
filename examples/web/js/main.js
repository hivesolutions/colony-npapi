// Hive Colony Framework
// Copyright (c) 2008-2020 Hive Solutions Lda.
//
// This file is part of Hive Colony Framework.
//
// Hive Colony Framework is free software: you can redistribute it and/or modify
// it under the terms of the Apache License as published by the Apache
// Foundation, either version 2.0 of the License, or (at your option) any
// later version.
//
// Hive Colony Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// Apache License for more details.
//
// You should have received a copy of the Apache License along with
// Hive Colony Framework. If not, see <http://www.apache.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

function version() {
    try {
        var gateway = document.getElementById("colony-gateway");
        alert(gateway.version());
    } catch (exception) {
        alert(exception);
    }
}

function pformat() {
    try {
        var gateway = document.getElementById("colony-gateway");
        alert(gateway.pformat());
    } catch (exception) {
        alert(exception);
    }
}

function pdevices() {
    try {
        var gateway = document.getElementById("colony-gateway");
        var devices = gateway.pdevices();
        var devicesS = "";
        var isFirst = true;
        for (var index = 0; index < devices.length; index++) {
            var device = devices[index];
            devicesS += isFirst ? "" : "\n"
            devicesS += device.name + " - " + (device.media || "N/A");
            isFirst = false;
        }
        alert(devicesS);
    } catch (exception) {
        alert(exception);
    }
}

function foo() {
    try {
        var gateway = document.getElementById("colony-gateway");
        alert(gateway.foo());
    } catch (exception) {
        alert(exception);
    }
}

function callback() {
    try {
        var gateway = document.getElementById("colony-gateway");
        gateway.callback(function(message) {
            alert(message);
        });
    } catch (exception) {
        alert(exception);
    }
}

function _alert() {
    try {
        var gateway = document.getElementById("colony-gateway");
        gateway.alert("Hello World :: Olá Mundo");
    } catch (exception) {
        alert(exception);
    }
}

function _print() {
    try {
        var gateway = document.getElementById("colony-gateway");
        var format = gateway.pformat();
        switch (format) {
            case "binie":
                gateway.print(true, HELLO_WORD_BINIE_B64);
                break;

            case "pdf":
                gateway.print(true, HELLO_WORD_PDF_B64);
                break;

            default:
                break;
        }
    } catch (exception) {
        alert(exception);
    }
}
