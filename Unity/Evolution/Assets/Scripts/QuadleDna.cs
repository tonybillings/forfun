using System;

public struct QuadleDna
{
    public int ArmTop { get; private set; }
    public int ArmRight { get; private set; }
    public int ArmBottom { get; private set; }
    public int ArmLeft { get; private set; }

    public QuadleDna(int armTop, int armRight, int armBottom, int armLeft)
    {
        ArmTop = armTop;
        ArmRight = armRight;
        ArmBottom = armBottom;
        ArmLeft = armLeft;
    }

    public static bool operator ==(QuadleDna a, QuadleDna b)
    {
        return (a.ArmTop == b.ArmTop && a.ArmRight == b.ArmRight && a.ArmBottom == b.ArmBottom && a.ArmLeft == b.ArmLeft);
    }

    public static bool operator !=(QuadleDna a, QuadleDna b)
    {
        return !(a == b);
    }

    public override bool Equals(object obj)
    {
        QuadleDna? other = obj as QuadleDna?;
        if (other == null) return false;
        return this == other;
    }

    public override int GetHashCode()
    {
        return ArmTop.GetHashCode() ^ ArmRight.GetHashCode() ^ ArmBottom.GetHashCode() ^ ArmLeft.GetHashCode();
    }

    public override string ToString()
    {
        return String.Join(",", new string[] { ArmTop.ToString(), ArmRight.ToString(), ArmBottom.ToString(), ArmLeft.ToString() });
    }
}
