﻿using NUnit.Framework;

namespace Inflames2K.Tests.MyListTests
{
    [TestFixture]
    public class IndexOf
    {
        [Test]
        public void Empty_list___returns_minus_1()
        {
            var sut = new MyList<int>();

            int actual = sut.IndexOf(42);

            Assert.AreEqual(-1, actual);
        }
        //---------------------------------------------------------------------
        [Test]
        public void Item_is_contained___returns_index()
        {
            var sut = new MyList<int>();
            sut.Add(2);
            sut.Add(1);

            Assume.That(sut.Count, Is.EqualTo(2));

            int actual = sut.IndexOf(1);

            Assert.AreEqual(1, actual);
        }
        //---------------------------------------------------------------------
        [Test]
        public void Item_is_contained_in_lower_half___returns_index()
        {
            var sut = new MyList<int>();
            sut.Add(2);
            sut.Add(1);
            sut.Add(3);
            sut.Add(4);
            sut.Add(5);

            Assume.That(sut.Count, Is.EqualTo(5));

            int actual = sut.IndexOf(1);

            Assert.AreEqual(1, actual);
        }
        //---------------------------------------------------------------------
        [Test]
        public void Item_is_contained_in_upper_half___returns_index()
        {
            var sut = new MyList<int>();
            sut.Add(2);
            sut.Add(1);
            sut.Add(3);
            sut.Add(4);
            sut.Add(5);

            Assume.That(sut.Count, Is.EqualTo(5));

            int actual = sut.IndexOf(4);

            Assert.AreEqual(3, actual);
        }
        //---------------------------------------------------------------------
        [Test]
        public void Item_is_not_contained___returns_minus_1([Values(0, -1, int.MinValue, int.MaxValue, 42)]int value)
        {
            var sut = new MyList<int>();
            sut.Add(1);
            sut.Add(2);

            Assume.That(sut.Count, Is.EqualTo(2));

            int actual = sut.IndexOf(value);

            Assert.AreEqual(-1, actual);
        }
        //---------------------------------------------------------------------
        [Test]
        public void Item_is_null___returns_index()
        {
            var sut = new MyList<object>();
            sut.Add(new object());
            sut.Add(null);

            Assume.That(sut.Count, Is.EqualTo(2));

            int actual = sut.IndexOf(null);

            Assert.AreEqual(1, actual);
        }
    }
}
