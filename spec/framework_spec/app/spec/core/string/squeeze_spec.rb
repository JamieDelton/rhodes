require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes.rb'

describe "String#squeeze" do
  it "returns new string where runs of the same character are replaced by a single character when no args are given" do
    "yellow moon".squeeze.should == "yelow mon"
  end

  it "only squeezes chars that are in the intersection of all sets given" do
    "woot squeeze cheese".squeeze("eost", "queo").should == "wot squeze chese"
    "  now   is  the".squeeze(" ").should == " now is the"
  end

  it "negates sets starting with ^" do
    s = "<<subbookkeeper!!!>>"
    s.squeeze("beko", "^e").should == s.squeeze("bko")
    s.squeeze("^<bek!>").should == s.squeeze("o")
    s.squeeze("^o").should == s.squeeze("<bek!>")
    s.squeeze("^").should == s
    "^__^".squeeze("^^").should == "^_^"
    "((^^__^^))".squeeze("_^").should == "((^_^))"
  end

  it "squeezes all chars in a sequence" do
    s = "--subbookkeeper--"
    s.squeeze("\x00-\xFF").should == s.squeeze
    s.squeeze("bk-o").should == s.squeeze("bklmno")
    s.squeeze("b-e").should == s.squeeze("bcde")
    s.squeeze("e-").should == "-subbookkeper-"
    s.squeeze("-e").should == "-subbookkeper-"
    s.squeeze("---").should == "-subbookkeeper-"
    "ook--001122".squeeze("--2").should == "ook-012"
    "ook--(())".squeeze("(--").should == "ook-()"
    s.squeeze("e-b").should == s
    s.squeeze("^e-b").should == s.squeeze
    s.squeeze("^b-e").should == "-subbokeeper-"
    "^^__^^".squeeze("^^-^").should == "^^_^^"
    "^^--^^".squeeze("^---").should == "^--^"

    s.squeeze("b-dk-o-").should == "-subokeeper-"
    s.squeeze("-b-dk-o").should == "-subokeeper-"
    s.squeeze("b-d-k-o").should == "-subokeeper-"

    s.squeeze("bc-e").should == "--subookkeper--"
    s.squeeze("^bc-e").should == "-subbokeeper-"

    "AABBCCaabbcc[[]]".squeeze("A-a").should == "ABCabbcc[]"
  end

  it "taints the result when self is tainted" do
    "hello".taint.squeeze("e").tainted?.should == true
    "hello".taint.squeeze("a-z").tainted?.should == true

    "hello".squeeze("e".taint).tainted?.should == false
    "hello".squeeze("l".taint).tainted?.should == false
  end

  it "tries to convert each set arg to a string using to_str" do
    other_string = mock('lo')
    other_string.should_receive(:to_str).and_return("lo")

    other_string2 = mock('o')
    other_string2.should_receive(:to_str).and_return("o")

    "hello room".squeeze(other_string, other_string2).should == "hello rom"
  end

  it "raises a TypeError when one set arg can't be converted to a string" do
    lambda { "hello world".squeeze([])        }.should raise_error(TypeError)
    lambda { "hello world".squeeze(Object.new)}.should raise_error(TypeError)
    lambda { "hello world".squeeze(mock('x')) }.should raise_error(TypeError)
  end

  it "returns subclass instances when called on a subclass" do
    StringSpecs::MyString.new("oh no!!!").squeeze("!").class.should == StringSpecs::MyString
  end
end

describe "String#squeeze!" do
  it "modifies self in place and returns self" do
    a = "yellow moon"
    a.squeeze!.should equal(a)
    a.should == "yelow mon"
  end

  it "returns nil if no modifications were made" do
    a = "squeeze"
    a.squeeze!("u", "sq").should == nil
    a.squeeze!("q").should == nil
    a.should == "squeeze"
  end

  ruby_version_is ""..."1.9" do
    it "raises a TypeError when self is frozen" do
      a = "yellow moon"
      a.freeze

      lambda { a.squeeze!("") }.should raise_error(TypeError)
      lambda { a.squeeze!     }.should raise_error(TypeError)
    end
  end

  ruby_version_is "1.9" do
    it "raises a RuntimeError when self is frozen" do
      a = "yellow moon"
      a.freeze

      lambda { a.squeeze!("") }.should raise_error(RuntimeError)
      lambda { a.squeeze!     }.should raise_error(RuntimeError)
    end
  end
end
