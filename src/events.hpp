/**
events.hpp
--------------
Defines event listener interface
 */

#pragma once

#include "common.hpp"

struct Trade;
struct PriceLevel;
struct TopOfBook;

struct IEventListener {
  virtual ~IEventListener() = default;
  virtual void on_ack(int) = 0;
  virtual void on_reject(int, RejectReason) = 0;
  virtual void on_cancel(int, CancelResult) = 0;
  virtual void on_trade(const Trade&) = 0;
  virtual void on_tob(const TopOfBook&) = 0;
  virtual void on_book(const BookSnapshot&) = 0;
};

  