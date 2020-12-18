//
// Copyright (c) 2010-2020 Antmicro
//
// This file is licensed under the MIT License.
// Full license text is available in 'licenses/MIT.txt'.
//
using System.Collections.Generic;
using Antmicro.Renode.Core;
using Antmicro.Renode.Core.Structure.Registers;
using Antmicro.Renode.Peripherals.Bus;
using Antmicro.Renode.Time;

namespace Antmicro.Renode.Peripherals.Timers
{
    // This class does not implement advanced-control timers interrupts
    [AllowedTranslations(AllowedTranslation.ByteToDoubleWord)]
    public class STM32F0Timer6 : LimitTimer, IDoubleWordPeripheral
    {
        public STM32F0Timer6(Machine machine, long frequency, uint initialLimit) : base(machine.ClockSource, frequency, limit: initialLimit,  direction: Direction.Ascending, enabled: false, autoUpdate: false)
        {
            IRQ = new GPIO();
            this.initialLimit = initialLimit;

            LimitReached += delegate
            {
                if(updateDisable.Value)
                {
                    return;
                }
                if(updateInterruptEnable.Value)
                {
                    IRQ.Set();
                    updateInterruptFlag = true;
                }
                Limit = autoReloadValue;
            };

            var registersMap = new Dictionary<long, DoubleWordRegister>
            {
                {(long)Registers.Control1, new DoubleWordRegister(this)
                    .WithFlag(0, writeCallback: (_, val) => Enabled = val, valueProviderCallback: _ => Enabled, name: "Counter enable (CEN)")
                    .WithFlag(1, out updateDisable, name: "Update disable (UDIS)")
                    .WithFlag(2, out updateRequestSource, name: "Update request source (URS)")
                    .WithFlag(3, writeCallback: (_, val) => Mode = val ? WorkMode.OneShot : WorkMode.Periodic, valueProviderCallback: _ => Mode == WorkMode.OneShot, name: "One-pulse mode (OPM)")
                    .WithReservedBits(4, 3)
                    .WithFlag(7, out autoReloadPreloadEnable, name: "Auto-reload preload enable (APRE)")
                    .WithReservedBits(8, 24)
                },

                {(long)Registers.DmaOrInterruptEnable, new DoubleWordRegister(this)
                    .WithFlag(0, out updateInterruptEnable, name: "Update interrupt enable (UIE)")
                    .WithReservedBits(1, 7)
                    .WithTag("Update DMA request enable (UDE)", 8, 1)
                    .WithReservedBits(9, 23)
                },

                {(long)Registers.Status, new DoubleWordRegister(this)
                    .WithFlag(0, FieldMode.Read | FieldMode.WriteZeroToClear,
                        writeCallback: (_, val) =>
                        {
                            if(!val)
                            {
                                updateInterruptFlag = false;
                                IRQ.Unset();
                            }
                        },
                        valueProviderCallback: (_) =>
                        {
                            return updateInterruptFlag;
                        },
                        name: "Update interrupt flag (UIF)")
                    .WithValueField(1, 31, writeCallback: (_, val) => val = val, valueProviderCallback: _ => (uint)Value, name: "Werd AF (WAF)")
                },

                {(long)Registers.EventGeneration, new DoubleWordRegister(this)
                    .WithFlag(0, FieldMode.WriteOneToClear, writeCallback: (_, val) =>
                    {
                        if(updateDisable.Value)
                        {
                            return;
                        }
                        if(Direction == Direction.Ascending)
                        {
                            Value = 0;
                        }
                        else if(Direction == Direction.Descending)
                        {
                            Value = autoReloadValue;
                        }
                        if(!updateRequestSource.Value && updateInterruptEnable.Value)
                        {
                            IRQ.Set();
                            updateInterruptFlag = true;
                        }
                    }, name: "Update generation (UG)")
                    .WithReservedBits(1, 31)
                },

                {(long)Registers.Counter, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => Value = val, valueProviderCallback: _ => (uint)Value, name: "Counter value (CNT)")
                },

                {(long)Registers.Prescaler, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => Divider = (int)val + 1, valueProviderCallback: _ => (uint)Divider - 1, name: "Prescaler value (PSC)")
                },

                {(long)Registers.AutoReload, new DoubleWordRegister(this)
                    .WithValueField(0, 32, writeCallback: (_, val) => autoReloadValue = val, valueProviderCallback: _ => autoReloadValue, name: "Auto-reload value (PSC)")
                }
            };

            registers = new DoubleWordRegisterCollection(this, registersMap);
            Reset();

            EventEnabled = true;
        }

        public uint ReadDoubleWord(long offset)
        {
            return registers.Read(offset);
        }

        public void WriteDoubleWord(long offset, uint value)
        {
            registers.Write(offset, value);
        }

        public override void Reset()
        {
            base.Reset();
            registers.Reset();
            IRQ.Set(false);
            autoReloadValue = initialLimit;
            Limit = initialLimit;
            updateInterruptFlag = false;
        }

        public GPIO IRQ { get; private set; }

        public long Size = 0x400;

        private uint initialLimit;
        private uint autoReloadValue;
        private bool updateInterruptFlag;
        private readonly IFlagRegisterField updateDisable;
        private readonly IFlagRegisterField updateRequestSource;
        private readonly IFlagRegisterField updateInterruptEnable;
        private readonly IFlagRegisterField autoReloadPreloadEnable;
        private readonly DoubleWordRegisterCollection registers;

        private enum Registers : long
        {
            Control1 = 0x0,
            Control2 = 0x04,
            DmaOrInterruptEnable = 0x0C,
            Status = 0x10,
            EventGeneration = 0x14,
            Counter = 0x24,
            Prescaler = 0x28,
            AutoReload = 0x2C,
        }
    }
}

