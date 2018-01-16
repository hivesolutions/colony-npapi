// Hive Colony Framework
// Copyright (c) 2008-2018 Hive Solutions Lda.
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
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008-2018 Hive Solutions Lda.
// __license__   = Apache License, Version 2.0

var HELLO_WORD_BINIE_B64 =
    "aGVsbG9fd29ybGQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAEAAABAAQAAAAAAAAAAAABDYWxp\
YnJpAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAACQAAAAMAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAwAAABIZWxsbyBXb3JsZAA="

var HELLO_WORD_PDF_B64 =
    "JVBERi0xLjQKJcfsj6IKNSAwIG9iago8PC9MZW5ndGggNiAwIFIvRmlsdGVyIC9G\
bGF0ZURlY29kZT4+CnN0cmVhbQp4nE2LPQvCMBCG8VvuV9yYDD1zl7SXrIIIbpZs\
4lSwU4fq/wfbopD3XR7ejxEdsaCb/YdugFOr2H9gibG9/uDdwwiR/KwlKLkb8Jyn\
Y0Ilr5IE8wt4qRhFE0nA6GtygTEP8DAry0I+spq1rZjYS1CzsUxBnUhBW+sopKZJ\
anbzUmNkNnvLTC6KL/uDrYTqFLX8H23tG2rkmW9wyXCf/AX5my5yZW5kc3RyZWFt\
CmVuZG9iago2IDAgb2JqCjE2MwplbmRvYmoKNCAwIG9iago8PC9UeXBlL1BhZ2Uv\
TWVkaWFCb3ggWzAgMCA1OTUgODQyXQovUm90YXRlIDAvUGFyZW50IDMgMCBSCi9S\
ZXNvdXJjZXM8PC9Qcm9jU2V0Wy9QREYgL1RleHRdCi9FeHRHU3RhdGUgMTAgMCBS\
Ci9Gb250IDExIDAgUgo+PgovQ29udGVudHMgNSAwIFIKPj4KZW5kb2JqCjMgMCBv\
YmoKPDwgL1R5cGUgL1BhZ2VzIC9LaWRzIFsKNCAwIFIKXSAvQ291bnQgMQovUm90\
YXRlIDA+PgplbmRvYmoKMSAwIG9iago8PC9UeXBlIC9DYXRhbG9nIC9QYWdlcyAz\
IDAgUgo+PgplbmRvYmoKNyAwIG9iago8PC9UeXBlL0V4dEdTdGF0ZQovT1BNIDE+\
PmVuZG9iagoxMCAwIG9iago8PC9SNwo3IDAgUj4+CmVuZG9iagoxMSAwIG9iago8\
PC9SOQo5IDAgUj4+CmVuZG9iagoxMiAwIG9iago8PC9MZW5ndGgxIDIxNjQwL0Zp\
bHRlci9GbGF0ZURlY29kZS9MZW5ndGggMTMgMCBSPj5zdHJlYW0KeJztnAl4FMW2\
x091z2T2zGRfJsuEzoQlIZONQCCSIRsJMSxJRmdYEwIIXjExEDYF4vUq3Ajivguo\
uEalM6AGQUDFFRAX3MAFBRWUCFfBBUjmneoziQHlvvv57rvP73vpmn//qk6dqq4+\
p7qHgBEYAJigCUQYO6bSkQHKUb4OTxfVzq6p97f74Wly7by5NkfpoFKsfwwQoJ1R\
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\
wrFJFEXB64xft1UUYCNqB0oUgdZyJaGSsl5BKxtHGEsYQxhNKCdcSCgjjCKUEkoI\
IwnFhCJCIaEPIYEWbyPEE+IIsYQYgpUQTYgiRNJtRhDCnfcgO1BnUKdRp1C/YIJ/\
Rv2E+hF1EnUC9QNm9XvUP1Bfo75CfYk6hDqI+gL1OWZ3N2oXaifqDdTrqNdQr6Je\
Qb2M2oF6CdWGehYz/gzqadRG1AbUPTz7QgfFeDHhKsIsbxD+UYjNJFxCYZlBmE6Y\
RqglTCXUEKoJUwiTCZMIEwkTCOMJHoKbcDHhIoKLUEVwEFIp1AMJKYRkwgBCf0I/\
Ql9CEsFOuUkkSAQ1QUUQCQKB0RMJzgeQPlQn6jAG9n3Ue6i9qHdR76DeRr2F2oN6\
EwO9CXWtaI//m5gafw1Ljf9rSZPr6pYm19KSxa4lLYtdhsXDFpctFg2LrYgrF7cs\
3r844KqSRa4rWxa5VItCFwn6hSXzXQta5rsM85lxXkmjq6rxUOOJRjG0sapxWuPc\
xlsb96JBs65xY+OORrHNt90Z3DhkWHFT442NQij2C9DIzNyc0GgILJ5b0uCa09Lg\
UjVkNQjDTjSwAw1MSGtgYxuqGwT02tCQ2K+Yew9qCI8utjSkNTgbxCtK6lz1LXWu\
MXV1dUvr1tRtq1MvrVtVJ6zHmuCs05mKLy+Z7fpsNoMtgg8sqO2Czyvq6zYLncDg\
mNDp9LG/YAAuxUDMSr3ENbPlEteM1Gmu6S3TXLWpU101qdWuKamTXJNbJrkmpo53\
TWgZ7/Kkul0Xo/9FqVUuV0uVqzJ1nKuiZZxrTOpo12i0l6eWuS5sKXONSi1xlbaU\
uMaWsJGpxa4iMTsev0EgDj/1cU1xx+NUhurY+lihPvZA7PFYsT7meIyw1MrM0Uuj\
V0WLZjwJdIqKj1oVtSZqfZTarFREY31wU7BQH9QUJKQFOYPeCjoQpIKgtUGCeZV5\
jXm9WRxjnmI+ZvaZVevNbH3gtsA9geKYwCmBdYGiOZC3RYszMDW92GyKNzlHOkxi\
rsOUZxpjEleZmNOUmlHsNCX2Lc4zjjFOMYprjMxpTOpffEzv0wtOPXYc0/l0gk/H\
QGQ2xoBZEKKW54iFxRfjftwQztQM/2jRWlWZnFzWpvFVlMnasRNktly2V/Kzc9x4\
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\
UyEAovkvSoMWUlgcq2OLsWcuW8GiQQX8/0jG/7kNxxzX+nx4ZvyMbRWez/VXfuEa\
gvEaAvBfzNYCVFWlDyqam5am9AC7kf8Xff/ioT27eRyO+87+1W6CKvBXsT1/fqmN\
sLJX55fGCcm96tWfTaoyKDtLGhjVq/+Z2E+w5I9IVQ4lf1TCh7DE/913/oN/t6jb\
Klrl9c9NMeeehCj6Mtr87VW7OHfu2zj/9L6OlbqjmpfRV6d83+HxX2DoFvoKZW5k\
c3RyZWFtCmVuZG9iagoxMyAwIG9iago2Mzg5CmVuZG9iagoxNCAwIG9iago8PC9G\
aWx0ZXIvRmxhdGVEZWNvZGUvTGVuZ3RoIDE1IDAgUj4+c3RyZWFtCnicXZAxDsIw\
DEX3nCI3qFvatAPyAksHEBJcIE0dlIE0CmXg9tQ1ZWB4kZ7yLfm7OPTHPoZZF5c8\
uSvN2oc4ZnpOr+xID3QPUZWVHoObv7a+7mGTKg4nm27vRHoJkFelJNw00jNZR9nG\
O6k9AO69R0Vx/PvqZGDwW7JEAeoOF61QANOw7lAA41hrFMB41gYFqIDVoABNy9qi\
AG3F2qEApl4X21bgHbkaNzvbB2n3ypnivPZf+3LTEOl3ojQlntIL6gOJQGNXCmVu\
ZHN0cmVhbQplbmRvYmoKMTUgMCBvYmoKMTk3CmVuZG9iago5IDAgb2JqCjw8L0Jh\
c2VGb250L1JGSENXVitUVDE1RXQwMC9Gb250RGVzY3JpcHRvciA4IDAgUi9Ub1Vu\
aWNvZGUgMTQgMCBSL1R5cGUvRm9udAovRmlyc3RDaGFyIDEvTGFzdENoYXIgOC9X\
aWR0aHNbIDYzMSA1MDMgMjQ2IDUzOCAyMjYgOTA2IDM1NSA1MzddCi9FbmNvZGlu\
ZyAxNiAwIFIvU3VidHlwZS9UcnVlVHlwZT4+CmVuZG9iagoxNiAwIG9iago8PC9U\
eXBlL0VuY29kaW5nL0Jhc2VFbmNvZGluZy9XaW5BbnNpRW5jb2RpbmcvRGlmZmVy\
ZW5jZXNbCjEvZzQ0L2cyODYvZzM2Ny9nMzgxL2czL2cxMTYvZzM5Ni9nMjgyXT4+\
CmVuZG9iago4IDAgb2JqCjw8L1R5cGUvRm9udERlc2NyaXB0b3IvRm9udE5hbWUv\
UkZIQ1dWK1RUMTVFdDAwL0ZvbnRCQm94WzAgLTExIDg4NCA2NzldL0ZsYWdzIDQK\
L0FzY2VudCA2NzkKL0NhcEhlaWdodCA2NzkKL0Rlc2NlbnQgLTExCi9JdGFsaWNB\
bmdsZSAwCi9TdGVtViAxMzIKL01pc3NpbmdXaWR0aCA1MDYKL0ZvbnRGaWxlMiAx\
MiAwIFI+PgplbmRvYmoKMiAwIG9iago8PC9Qcm9kdWNlcihHUEwgR2hvc3RzY3Jp\
cHQgOC4xNSkKL0NyZWF0aW9uRGF0ZShEOjIwMTMwMTMwMTEyNzE0KQovTW9kRGF0\
ZShEOjIwMTMwMTMwMTEyNzE0KQovVGl0bGUoaGVsbG9fd29ybGQpCi9DcmVhdG9y\
KFBTY3JpcHQ1LmRsbCBWZXJzaW9uIDUuMi4yKQovQXV0aG9yKGpvYW1hZyk+PmVu\
ZG9iagp4cmVmCjAgMTcKMDAwMDAwMDAwMCA2NTUzNSBmIAowMDAwMDAwNDk1IDAw\
MDAwIG4gCjAwMDAwMDc5MzUgMDAwMDAgbiAKMDAwMDAwMDQyNyAwMDAwMCBuIAow\
MDAwMDAwMjY3IDAwMDAwIG4gCjAwMDAwMDAwMTUgMDAwMDAgbiAKMDAwMDAwMDI0\
OCAwMDAwMCBuIAowMDAwMDAwNTQzIDAwMDAwIG4gCjAwMDAwMDc3MzYgMDAwMDAg\
biAKMDAwMDAwNzQyNyAwMDAwMCBuIAowMDAwMDAwNTg0IDAwMDAwIG4gCjAwMDAw\
MDA2MTQgMDAwMDAgbiAKMDAwMDAwMDY0NCAwMDAwMCBuIAowMDAwMDA3MTE4IDAw\
MDAwIG4gCjAwMDAwMDcxMzkgMDAwMDAgbiAKMDAwMDAwNzQwNyAwMDAwMCBuIAow\
MDAwMDA3NjE5IDAwMDAwIG4gCnRyYWlsZXIKPDwgL1NpemUgMTcgL1Jvb3QgMSAw\
IFIgL0luZm8gMiAwIFIKL0lEIFsoW1xcnXO+nIkmQYEJ0OGudKopKFtcXJ1zvpyJ\
JkGBCdDhrnSqKV0KPj4Kc3RhcnR4cmVmCjgxMTcKJSVFT0YKCgo0IDAgb2JqCjw8\
Ci9Db250ZW50cyA1IDAgUgovTWVkaWFCb3ggWyAwIDAgNTk1IDg0MiBdCi9QYXJl\
bnQgMyAwIFIKL1Jlc291cmNlcyA8PAovRXh0R1N0YXRlIDEwIDAgUgovRm9udCAx\
MSAwIFIKL1Byb2NTZXQgWyAvUERGIC9UZXh0IF0KPj4KL1JvdGF0ZSAwCi9UeXBl\
IC9QYWdlCj4+CmVuZG9iagoyIDAgb2JqCjw8Ci9BdXRob3IgKEhpdmUgU29sdXRp\
b25zIExkYS4pCi9Nb2REYXRlIChEOjIwMTMwMTMwMTE0NDMyWikKL1RpdGxlIChI\
ZWxsbyBXb3JsZCkKPj4KZW5kb2JqCgp4cmVmCjIgMQowMDAwMDA4NzUzIDAwMDAw\
IG4NCjQgMQowMDAwMDA4NTgyIDAwMDAwIG4NCgp0cmFpbGVyCjw8Ci9JRCBbIDw1\
QjVDOUQ3M0JFOUM4OTI2NDE4MTA5RDBFMUFFNzRBQT4gPDhEOThDRDRFOEZCREQz\
QkE2OUEwMTg0Q0E0MjRGMzFFPiBdCi9JbmZvIDIgMCBSCi9QcmV2IDgxMTcKL1Jv\
b3QgMSAwIFIKL1NpemUgMTcKPj4Kc3RhcnR4cmVmCjg4NTUKJSVFT0Y="
