// Hive Colony Framework
// Copyright (C) 2008-2012 Hive Solutions Lda.
//
// This file is part of Hive Colony Framework.
//
// Hive Colony Framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Colony Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Colony Framework. If not, see <http://www.gnu.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2012 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

function version() {
    try {
        var gateway = document.getElementById("colony-gateway");
        alert(gateway.version());
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
        gateway.alert("João Magalhães");
    } catch (exception) {
        alert(exception);
    }
}

function _print() {
    try {
        var gateway = document.getElementById("colony-gateway");
        gateway.print(true);
    } catch (exception) {
        alert(exception);
    }
}
