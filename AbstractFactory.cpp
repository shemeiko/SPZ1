#include <iostream>
#include <memory>
#include <format>
#include <string>

class AbstractPayment {
public:
    virtual std::string GetInvoice() const = 0; 
    virtual ~AbstractPayment() = default;
};

class AbstractTransaction {
public:
    virtual std::string Status() const = 0;
    virtual ~AbstractTransaction() = default;
};

class VisaPayment : public AbstractPayment {
    static int count;
public:
    VisaPayment() { count++; }
    std::string GetInvoice() const override {
        return std::format("[Visa]: Payment #{} has been created by Visa", count);
    }
};

int VisaPayment::count = 0;

class MastercardPayment : public AbstractPayment {
    static int count;

public:
    MastercardPayment() { count++; }
    std::string GetInvoice() const override {
        return std::format("[Mastercard]: Payment #{} has been created by Mastercard", count);
    }
};

int MastercardPayment::count = 0;

class VisaTransaction : public AbstractTransaction {
public:
    std::string Status() const override {
        return "[Visa]: Transactions had been logged!";
    }
};

class MastercardTransaction : public AbstractTransaction {
public:
    std::string Status() const override {
        return "[Mastercard]: Transactions had been logged!";
    }
};

class AbstractPaymentProviderFactory {
public:
    virtual std::unique_ptr<AbstractPayment> CreatePayment() const = 0;
    virtual std::unique_ptr<AbstractTransaction> CreateTransaction() const = 0;
    virtual ~AbstractPaymentProviderFactory() = default;
};

class VisaPaymentProviderFactory : public AbstractPaymentProviderFactory {
public:
    std::unique_ptr<AbstractPayment> CreatePayment() const override {
        return std::make_unique<VisaPayment>();
    }
    std::unique_ptr<AbstractTransaction> CreateTransaction() const override {
        return std::make_unique<VisaTransaction>();
    }
};

class MastercardPaymentProviderFactory : public AbstractPaymentProviderFactory {
public:
    std::unique_ptr<AbstractPayment> CreatePayment() const override {
        return std::make_unique<MastercardPayment>();
    }
    std::unique_ptr<AbstractTransaction> CreateTransaction() const override {
        return std::make_unique<MastercardTransaction>();
    }
};

void ClientCode(const AbstractPaymentProviderFactory& factory) {
    std::cout << "------Testing code------" << '\n';
    auto transaction = factory.CreateTransaction();
    for (int i = 0; i < 5; ++i) std::cout << factory.CreatePayment()->GetInvoice() << '\n';
    std::cout << transaction->Status() << "\n\n";
}

int main() {
    std::unique_ptr<AbstractPaymentProviderFactory> factory = std::make_unique<VisaPaymentProviderFactory>();
    ClientCode(*factory);
    factory = std::make_unique<MastercardPaymentProviderFactory>();
    ClientCode(*factory);

    return 0;
}