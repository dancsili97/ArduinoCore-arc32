/*
 * Copyright (c) 2015 Intel Corporation.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "BleCentral.h"

#include "BlePeripheral.h"


BleCentral::BleCentral(BlePeripheral* peripheral) :
  _peripheral(peripheral)
{
    clearAddress();
}

BleCentral::operator bool() const {
    BleDeviceAddress zero;

    memset(&zero, 0, sizeof(zero));

    return (memcmp(&_address, &zero, sizeof(_address)) != 0);
}

bool BleCentral::operator==(const BleCentral& rhs) const {
    return (memcmp(&_address, &rhs._address, sizeof(_address)) == 0);
}

bool BleCentral::operator!=(const BleCentral& rhs) const {
    return !(*this == rhs);
}

bool BleCentral::connected() {
    poll();

    return (*this && *this == _peripheral->central());
}

const char* BleCentral::address() const {
    static String address = "";

    for (int i = 5; i >= 0; i--) {
        unsigned char a = _address.addr[i];

        if (a < 0x10) {
            address += "0";
        }

        address += String(a, 16);

        if (i > 0) {
            address += ":";
        }
    }

    return address.c_str();
}

void BleCentral::poll() {
    _peripheral->poll();
}

BleStatus BleCentral::disconnect() {
    if (connected()) {
        return _peripheral->disconnect();
    }

    return BLE_STATUS_WRONG_STATE;
}

void BleCentral::setAddress(BleDeviceAddress address) {
    _address = address;
}

void BleCentral::clearAddress() {
    memset(&_address, 0x00, sizeof(_address));
}
