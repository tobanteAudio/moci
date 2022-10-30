import pytest

import profile_viewer as pv

def test_standard_deviation():
    assert pv.standard_deviation([0]) == 0
    assert pv.standard_deviation([1,0]) == 0.5