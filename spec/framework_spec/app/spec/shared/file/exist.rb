describe :file_exist, :shared => true do
  it "return true if the file exist" do
    @object.send(@method, File.join(__rhoGetCurrentDir(), __FILE__).gsub(/\.rb/,".iseq")).should == true
    @object.send(@method, 'a_fake_file').should == false
  end

  it "return true if the file exist using the alias exists?" do
    @object.send(@method, File.join(__rhoGetCurrentDir(), __FILE__).gsub(/\.rb/,".iseq")).should == true
    @object.send(@method, 'a_fake_file').should == false
  end

  it "raises an ArgumentError if not passed one argument" do
    lambda { @object.send(@method) }.should raise_error(ArgumentError)
    lambda { @object.send(@method, File.join(__rhoGetCurrentDir(), __FILE__), File.join(__rhoGetCurrentDir(), __FILE__)) }.should raise_error(ArgumentError)
  end

  it "raises a TypeError if not passed a String type" do
    lambda { @object.send(@method, nil) }.should raise_error(TypeError)
  end
end
