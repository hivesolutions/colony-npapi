#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import base64
import shutil
import tempfile
import unittest

import npcolony


class GlobalTest(unittest.TestCase):

    def setUp(self):
        self.target_dir = tempfile.mkdtemp(prefix="npcolony-test-")
        self.data = b"%PDF-1.4 npcolony test document"
        self.data_b64 = base64.b64encode(self.data).decode("utf-8")

    def tearDown(self):
        shutil.rmtree(self.target_dir, ignore_errors=True)

    def test_basic(self):
        self.assertEqual(type(npcolony.VERSION), str)
        self.assertEqual(npcolony.VERSION, "1.2.10")

        self.assertEqual(type(npcolony.get_devices()), list)

    def test_get_devices(self):
        for device in npcolony.get_devices():
            self.assertEqual(
                sorted(device.keys()),
                [
                    "bottom",
                    "is_default",
                    "left",
                    "length",
                    "media",
                    "name",
                    "right",
                    "top",
                    "width",
                ],
            )
            self.assertEqual(device["left"] <= device["right"] <= device["width"], True)
            self.assertEqual(
                device["bottom"] <= device["top"] <= device["length"], True
            )

    @unittest.skipIf(os.name == "nt", "print to file writes the document on unix")
    def test_print_printer_base64_output_path(self):
        path = os.path.join(self.target_dir, "output.pdf")
        result = npcolony.print_printer_base64(
            "npcolony-test-printer", self.data_b64, options=dict(output_path=path)
        )
        self.assertEqual(result, 0)
        with open(path, "rb") as file:
            self.assertEqual(file.read(), self.data)

    @unittest.skipIf(os.name == "nt", "print to file writes the document on unix")
    def test_print_printer_base64_invalid_path(self):
        path = os.path.join(self.target_dir, "missing", "output.pdf")
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.data_b64, options=dict(output_path=path)
            ),
        )

    @unittest.skipIf(os.name == "nt", "printer resolution is only checked on unix")
    def test_print_printer_base64_unknown(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.data_b64
            ),
        )

    @unittest.skipIf(os.name == "nt", "printer resolution is only checked on unix")
    def test_print_printer_base64_invalid_options(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer",
                self.data_b64,
                options=dict(output_path=1, title=None, media=3.5),
            ),
        )
