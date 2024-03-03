// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017-2019 The Raven Core developers
// Copyright (c) 2023-2024 The Aidp Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef AIDP_QT_AIDPADDRESSVALIDATOR_H
#define AIDP_QT_AIDPADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class AidpAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AidpAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Aidp address widget validator, checks for a valid raven address.
 */
class AidpAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AidpAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // AIDP_QT_AIDPADDRESSVALIDATOR_H
