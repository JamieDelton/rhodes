describe :argf_each_line, :shared => true do
  before :each do
    @file1_name = fixture File.join(__rhoGetCurrentDir(), __FILE__), "file1.txt"
    @file2_name = fixture File.join(__rhoGetCurrentDir(), __FILE__), "file2.txt"

    @lines  = File.readlines @file1_name
    @lines += File.readlines @file2_name
  end

  after :each do
    ARGF.close
  end

  it "reads each line of files" do
    argv [@file1_name, @file2_name] do
      lines = []
      ARGF.send(@method) { |b| lines << b }
      lines.should == @lines
    end
  end
end
