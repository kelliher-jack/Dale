
(setf prinlevel 20)
(load mini)
(mini 'Dale)
dip: !<!-, !(!$, !$!) ;

Dale: DaleStatements @@# ;

DaleStatements: DaleStatement <DaleStatements> ;

DaleStatement: xfunction / xClass .(setf _presentIVar nil) /
               (Expression '; .(rprint (eval (list '_eval #1))));

Expressions: expression '; +.(_MkExpr #1)
                           +.(append #2 (list #1)) < expressions > ;

Statements: < Expressions >
            <'RETURN Expression '; +.(_MkRExpr #1) 
                                   +.(append #2 (list #1))> ;

Expression: ( Variable +.(list #1)
                       <( '<-  +.(append #1 '(assign))
                             Expression +.(append #2 (list #1)))
                         / ( MessageExpression 
                             < '<-   +.(append #1 '(assign))
                                Expression  +.(append #2 (list #1))>)
                         / parameter > )
            / ( primary +.(list #1) < MessageExpression > ) ;

MessageExpression: Keyword +.(append #2 (list #1))
                   < MessageExpression / Parameter > ;

Parameter: Primary +.(append #2 (list #1)) < MessageExpression > ;

Primary: Variable / Num  / Block +.(_MkBlock #1) / '( Expression ') ;

Variable: Id ;

TypeName: Id;

ClassId: Id;

Block: '($ +.(_startblock) < VariableDeclaration > Statements '$)
                                                     .(_EndBlock) ;

VariableDeclaration: 'VAR VariableList[';]*  '. .(!_MkVar #1);

VariableList: Variable <TypeName +.(list #2 #1)> ;

xClass: 'CLASS ClassId .(!_MkClass #1)
        < 'SUPERCLASS!: ClassId .(!_AddSuper #1)>
        < 'INSTANCE!-VARIABLES!: VariableList[';]*
                               '. .(!_MkInstance #1) >
        < 'CLASS 'METHODS!: .(setf !_domain 'class) 
                             xMethods* .(!_MkMethods !_domain)>
        'INSTANCE 'METHODS!: .(setf !_domain 'external)
                < DomainSwitch > xMethods* 
                                .(!_MkMethods !_domain)
                < DomainSwitch  xMethods* 
                                .(!_MkMethods !_domain)> 'ENDCLASS '. ;

xFunction: 'FUNCTION ClassId .(!_MkFctn #1)
          'METHOD!: .(setf !_domain 'class) 
                             fMethods .(!_MkMethods !_domain)
          'END '. ;

DomainSwitch: 'EXTERNAL .(setf !_domain 'external)
              / 'INTERNAL .(setf !_domain 'internal);

xMethods: MessagePattern .(_MkMsg #1) ( Block / xPrimitive ) .(_MkProc);

fMethods: ParameterPattern .(_MkMsg #1) (Block / xPrimitive) .(_MkProc);

MessagePattern: Keyword +.(list #1) < Vpattern / Epattern > ;

ParameterPattern: 'Self!-Msg +.(list '!<none!>) < Vpattern / Epattern >;

KeywordPattern: keyword +.(append #2 (list #1)) (Vpattern / Epattern)* ;

Evalarg: '$ Variable +.(append #2 (list #1)) ;

Valarg: '& variable +.(append #2 (list (list #1))) ;

Epattern: Evalarg < '$ keyword +.(append #2 (list #1))
                              / ( Keyword +.(append #2 (list #1))
                                          ( Vpattern / Epattern ))> ;

Vpattern: Valarg '&  keyword +.(append #2 (list #1))
                   < Vpattern / Epattern > ;

keyword: id ;  % nonnumeric (nonnumeric / digit)* ;

xPrimitive: 'PRIMITIVE sexp .(_MkCode #1) ;

sexp:  id / num / str / '( dotexp ') ;

dotexp: sexp* < '. sep +.(attach #2 #1) > ;

fin

(de attach (x y)
    (for (in z x)
       (do (setf y (cons z y))))
    y)

(de ConvertToStr (x)
  (eval (append 
         '(string)
         (cdr 
          (for (in u x)
               (join (append  '(32) (string2list (id2string u)))))))))

% ------ Global Variables and status states -----
% !_UnEvalArgs - a dale global var that contains the rest of a expr when
%                traped by an <ANY> message trap.
% !_presentDef - contains the name of the present class we are parsing.
% !_presentMethods - contains the list of methods
% !_presentMsgs - contains the list of message templates.
% !_presentIVar - contains the list of instance variables for a given
%                 class associated with there types and field offsets.
% !_currentMethod - contains the method that the parser is currently
%                   working on.
% !_currentBlock - contains the block the the parser is currently
%                   working on.
% !_domain - contains the current message domain (internal,
%            external or class).
% !_rtnflg - signals whether of not to generate a RETURN self clause.
%            nil means to generate it.
% !_Contex - contains the current class context in which a message is
%            to be evaluated during evaluation.
% !_OldSelf- Special global var to hold the last value of the dale
%            variable self used to fix reference bug in ifTrue: since
%            self would refer to a boolean object not the last contex
%            of self.
% !_ErrFlg - a boolean flag that is set to true if a runtime error is
%            found used to limit avanches of error messages.
% !_PrtFlg - a boolean flag that is set to true if user wants all dale
%            return objects to be printed as vectors.  else only 
%            integers and boolean objects are printed.
% !_TraceFlg a boolean flag that is set to true to give a program trace.

(setf !_TraceFlg nil)
(setf !_ErrFlg nil)
(setf !_PrtFlg t)
(setf !_Contex nil)
(setf !_OldSelf nil)
(setf !_presentDef nil)
(setf !_UnEvalArgs nil)
(setf !_presentMethods nil)
(setf !_presentMsgs nil)
(setf !_currentBlock nil)
(setf !_currentMethod nil)
(put 'class 'index 3)
(put 'external 'index 4)
(put 'internal 'index 5)

% ------  utility functions ------

(de current ()
  (get !_presentDef 'daleid))
    %returns the current class  description dale object %

(de traverse (atobject path)
  (for (in u path)
       (do (setf atobject (!_GetField atobject (eval u)))))
  atobject)
    % returns object at end of path where path is a list of field
    % indexes i.e. ((atobject[i1])[i2])[in]

(de _push (item)
  (setf stack (cons item stack)))

(de _pop ()
  (prog (s)
    (setf s (car stack))
    (setf stack (cdr stack))
    (return s)))

(de pushc (class)
  (setf _contex (cons class _contex)))

(de popc ()
  (setf _contex (cdr _contex)))

(de msgsplit (MList)
  (let ((oddEven T)(l1 nil)(l2 nil))
    (for (in u MList)
         (do (if oddEven (progn (setf oddEven nil)
                                (setf l1 (append l1 (list u))))
               (setf oddEven T)
               (if (atom u)
                 (progn
                  (setf l2 (append l2 (list u)))
                  (setf l1 (append l1 '(!_arg))))
                 (setf l2 (append l2 u))
                 (setf l1 (append l1 '(!_val)))))))
    (list l1 l2)))
    % splits a message list into an internal  form message list
    % and a parameter list.

(de find* (msg Class domain)
  (let* ((method (_GetField class domain))
         (TC (if method (find msg method)
               nil)))
    (cond
     ((Null (_GetField class 1))
      (if (Null TC) nil
        (append TC (list (_GetField class 6)))))
     ((Null TC) (let ((C (get (_GetField class 1) 'daleid)))
                  (if (null C) nil 
                    (find* msg C domain))))
     (t (append TC (list (_GetField class 6)))))))
        
%msg - message that is searched for
%class - the class in which to search for the msg
%domain - restriction on partitions of message types 
%  3 - class messages
%  4 - external instance methods
%  5 - internal instance methods
% TC - found template code pair
% C  - Next class to look in.

(de find (msg Methods)
  (car
   (for (from i 1 (!_getfield (!_getfield methods 1) 1) 1) % field 1 = 
        (when                                      % number of methods
          (or (equal (car (traverse methods (list (Times2 i 2) 1)))
                     msg)
              (equal (car (traverse methods (list (Times2 i 2) 1)))
                     '!<ANY!>)))
        (collect (list
                  (traverse methods (list (Times2 i 2) 1))
                  (traverse methods (list (PLUS2 
                                           (Times2 i 2) 1) 1)))))))
    %find looks for the primary message in the methods of a given class
    % and returns the list containing the message template and code.
    % if the primary message is not found the it returns nil.



% _EvalParse takes a  message expression and a parameter template
%      and returns the list of arguments extracted from the message
%      expression.  A fluid variable extraArgs is set to hold the
%      next message expression if any.
%      <none> is a special message delimiter which specifies that
%      no message is given in the expression.

(de !_evalParse (expr template)
  (let ((exprList nil)(temp template)(temp2 expr)(primary nil))
    (while (not (null template))
      (setf arglist nil)
      (setf primary (first template))
      (setf template (cdr template)) %move to next position in template
      (if 
        (and (null expr) (not (equal primary '!<none!>)))
        (dale-error "missing message delimiter" nil self)
        (if (not (equal primary '!<none!>))
          (setf expr (cdr expr)))      %move to next token in expression
          % message was null so do nothing next token is param.
        (if (null template)
          (if (not (null expr))      % closing message
            (setf extraArgs expr))
          (if (null (cdr template))
            (let ((arg (get (car expr) 'daleid))) %trailing arguments
              (if (not (null arg))
                (if (equal (_GetClass arg) 'function)
                  (progn    % have a function collect its argument expr
                   (setf arg (length (first (find '!<none!> 
                                                  (_GetField arg 3)))))
                   (if (greaterP arg (length expr))
                     (dale-error "missing function arguments" nil self)
                     (setf extraArgs (PNth expr (add1 arg)))
                     (setf arglist (for (from i 1 arg)
                                        (collect (Nth expr i))))))
                  (progn (setf arglist (list (car expr)))
                         (setf extraArgs (cdr expr))))
                (progn (setf arglist (list (car expr)))
                       (setf extraArgs (cdr expr)))))
            (while (not (equal (second template) (car expr))) %enclosed
              (setf arglist (append arglist (list (car expr)))) %args
              (setf expr (cdr expr)))
            (if (equal (first template) '_val)  % non-evaluated args
              (prog (tmp)                 % make in to a dale string
               (setf tmp (list2vector (explode arglist)))
               (setf tmp (sub tmp 1 (plus2 (size tmp) -2)))
               (setf arglist
                 (_NewInstance 'string (plus2 1 (size tmp))))
               (setsub arglist 2 (size tmp))
               (setf arglist (list arglist)))))
          (setf template (cdr template))
          (if (null arglist)
            (dale-error "missing argument" nil self)
            (setf exprlist (append exprlist (list arglist)))))))
    (if (equal (car temp) '!<ANY!>) 
      (progn
       (setf extraArgs nil)
       (setf !_UnEvalArgs temp2)))
    exprlist))
      
    


% ------  parser called primitive functions ------

(de !_MkClass (name)
  % (printf "!_MkClass: %w%n" name)
  (put name 'daleid (!_NewInstance 'class 6))
  (_StoreField (get name 'daleid) 6 name)
  (setf !_presentDef name)
  (setf !_presentIVar nil) 
  (setf !_rtnflg nil)
  (setf !_presentMethods nil)  %clear current type methods lists %
  (setf !_presentMsgs nil)
  (setf !_currentBlock nil)
  (setf !_currentMethod nil)
  (setf stack nil))
    % Dictionary definition objects are not implemented as dale
    % objects but as properties on the definitions name that
    % point to the description of the object.
    % (let psl do the house keeping)

(de !_MkFctn (name)
  % (printf "!_MkCFctn: %w%n" name)
  (put name 'daleid (!_NewInstance 'function 6))
  (_StoreField (get name 'daleid) 6 name)
  (setf !_presentDef name)
  (setf !_presentIVar nil) 
  (setf !_rtnflg nil)
  (setf !_presentMethods nil)  %clear current type methods lists %
  (setf !_presentMsgs nil)
  (setf !_currentBlock nil)
  (setf !_currentMethod nil)
  (setf stack nil))
    % Dictionary definition objects are not implemented as dale
    % objects but as properties on the definitions name that
    % point to the description of the object.
    % (let psl do the house keeping)

(de !_AddSuper (name)
  % (printf "!_AddSuper: %w%n" name)
  (!_StoreField (current) 1 name))
    % adds super class to class object. %

(de !_MkInstance (IList)
  % (printf "!_MkInstance: %w%n" IList)
  % build global instance variable symbol table (!_presentIVar) %
  (let ((index 0))
    (setf !_presentIVar
      (for (in u IList)
           (collect
            (progn (setf index (plus2 index 1))
                   (if (atom u) (list u 'object index)
                     (list (car u) (second u) index)))))))
  (!_StoreField (current) 2
                (!_NewInstance 'field (PLUS (length IList) 1)))
  (!_StoreField (Traverse (current) '(2))
                1
                (!_NewInstance 'INTEGER 1))
  (!_StoreField (Traverse (current) '(2 1)) 1 (length IList))
  (let ((index 1))
    (for (in u IList)
         (do 
          (setf index (PLUS2 index '1))
          (!_StoreField (Traverse (current) '(2))
                        index
                        (!_NewInstance 'name 2))
          (!_StoreField (Traverse (current) '(2 index))
                        1
                        (cond ((atom u) u)
                              (t (car u))))
          (!_StoreField (Traverse (current) '(2 index))
                        2
                        (cond ((atom u) 'object)
                              (t (cadr u))))))))
                       
(de !_MkVar (VList)
  % (printf "!_MkVar: %w%n" VList)
  (setf !_currentblock
    (list 'Prog
          (append
           (for (in u VList)
                (collect (cond ((atom u) u)
                               (t (car u)))))
           (for (in u _presentIVar)
                (collect (first u))))))
  (for (in u VList)
       (do
        (setf !_currentblock
          (append !_currentblock
                  (list
                   (cond
                    ((atom u) (list 'setf u 
                                    '(!_NewInstance 'objectPointer 1)))
                    (t (list 'setf 
                             (car u) '(!_NewInstance 
                                       'objectPointer 1)))))))))
  (for (in u !_presentIVar)
       (do
        (setf !_currentblock
          (append !_currentblock
                  (list (list 'setf (first u) (third u))))))))
                        

(de !_MkMsg (MList)
  % (printf "!_MkMsg: %w%n" MList)
  (let ((M (msgsplit MList)))
    (setf !_presentMsgs (append !_presentMsgs (list (car M))))
    (setf !_currentMethod
      (list 'lambda 
            (append '(self) (cadr M))))))
    % save infomation from the Message list on presentmsgs global list.
    % start building the lambda expression for the methods code
    % (lambda (self arg1 arg2   argn))

(de !_MkMethods (methodtype)
  % (printf "!_MkMethods: %w%n" methodtype)
  (!_StoreField (current) (get !_domain 'index)
                (!_NewInstance 'methods
                               (PLUS2
                                (TIMES2 (length _presentMsgs) 2) 1)))
  (!_StoreField (Traverse (current) (list(get _domain 'index)))
                1
                (!_NewInstance 'INTEGER 1))
  (!_StoreField (Traverse (current) (list(get _domain 'index) 1))
                1
                (length _presentMsgs))
  (let ((index 1))
    (for (in u _presentMsgs) (in v _presentMethods)
         (do
          (setf index (PLUS2 index 1))
          (!_StoreField (Traverse (current) 
                                  (list (get _domain 'index)))
                        index
                        (!_NewInstance 'msgTemplate 1))
          (!_StoreField (Traverse (current)
                                  (list (get _domain 'index) index))
                        1
                        u)
          (!_StoreField (Traverse (current) 
                                  (list (get _domain 'index)))
                        (setf index (PLUS2 index 1))
                        (!_NewInstance 'code 1))
          (!_StoreField (Traverse (current)
                                  (list (get _domain 'index) index))
                        1
                        V))))
  (setf !_presentmsgs nil)     %reset method lists
  (setf !_presentmethods nil))

(de !_MkCode (LCode)
  % (printf "!_MkCode: %w%n" LCode)
  (setf _currentmethod
    (append _currentmethod (list LCode))))

(de !_MkExpr (Expr)
  % (printf "!_MkExpr: %w%n" Expr)
  (setf !_currentblock 
    (append !_currentblock 
            (list (list '!_eval Expr))))
  'Expr)

(de !_MkRExpr (Expr)
  % (printf "!_MkRExpr: %w%n" Expr)
  (setf !_rtnflg t)
  (setf !_currentblock 
    (append !_currentblock 
            (list (list 'Return (list '!_eval Expr)))))
  'Expr)

(de !_MkBlock (name)
  % (printf "!_MkBlock: %w%n" name)
  (let
    ((ref (_StoreField
           (_NewInstance 'code 1) 1 (lastcar _currentMethod))))
    (setf _currentMethod
      (for (in u _currentMethod)
           (unless (equal u (lastcar _currentMethod)))
           (collect u)))
    ref))

(de !_StartBlock ()
  % (printf "!_StartBlock:%n")
  (_push _currentBlock)
  (_push _rtnflg)
  (setf _rtnflg nil)
  (setf !_currentBlock
    (list 'Prog
          (for (in u _presentIVar)
               (collect (first u)))))
  (setf !_currentBlock
    (append !_currentblock
            (for (in u _presentIvar)
                 (collect (list 'setf (first u) (third u)))))))

(de !_EndBlock ()
  % (printf "!_EndBlock:%n")
  (if _rtnflg
    (setf _currentMethod
      (append _currentMethod (list _currentBlock)))
    (setf _currentblock
      (append _currentblock              (list (list 'RETURN (list '_eval '(self))))))
    (setf _currentMethod
      (append _currentMethod (list _currentBlock))))
  (setf _rtnflg (_pop))
  (setf _currentBlock (_pop)))

(df !_eval (expr)
  (_trace expr self)
  (let* ((expr (_ResolvePass (car expr))) % strip off extra ()'s
         (_object nil)                    % and resolve instances
         (_self (first expr)) % set _self to refer to the object
         (temp nil) (templateCode nil))
    (cond
     ((null expr)
      (progn (dale-error "object evaluated to nil" nil self)
             (_NewInstance 'Error 1)))
     ((null (cdr expr)) % no primary message
      (let ((expr (car expr)) (temp nil))
        (cond
         ((listP expr)
          (if (equal (first expr) '_IVar)
            (second expr)   % get resolved instance value
            (eval (list '_eval expr)))) % nested expr.
         ((numberP expr)
          (_StoreField (_NewInstance 'integer 1) 1 expr))
         ((vectorP expr) expr)
         ((not (UnboundP expr)) (eval expr)) % a loc Variable 
         (t (setf temp (get _self 'daleid))
            (if (NULL temp)  % not a object
              (dale-error "undefined object" expr self)
              temp)))))  % is a dale defined object
     (t (if (listP _self)
          (if (equal (first _self) '_IVar)
            (setf _object (second _self)) % resolved instance value.
            (setf _object (eval (list '_eval _self))))
          (setf _object (eval (list '_eval (list _self)))))
        (cond ((null _object)
               (dale-error "undefined object" (first expr) self))
              ((equal (!_GetClass _object) 'class) % the message is a
               (setf templateCode (find*  (second expr) % class method
                                          _object  3)))
              ((equal (!_GetClass _object) 'function)
               (setf templateCode (find* '!<none!> _object 3)))
              (t (setf temp (get (!_GetClass _object) 'daleid)) % the
                 (if (null temp)   % message is an instance method.
                   (Printf "Dale - %w's class '%w' does not exist%n"
                           (first expr) (_GetClass _object))
                   (setf templateCode (find* (second expr)
                                            temp 4)))))

        % _Instance-Ref objects need to know the name of the super class
        % whose method will be used.  Then  add index to field size
        % of that class's inherentance storage size.

        % we now have the template and code for the primary message. 
        % remember self will become _object on application of message.
        % note extraArgs is set to hold un-parsed args by _evalParse
        % these must the be evaluated to give left to right evaluation.
        (if (null templateCode)
          (dale-error "undefined message" (second expr) self)
          (pushc (third templateCode))
          (setf _OldSelf self)
          (let ((extraArgs nil)
                (right nil))
            (setf right
              (apply (second templateCode)
                     (cons _object
                           (for (in u (!_evalParse (cdr expr)
                                                   (car templateCode)))
                                (collect
                                 (sentinel (eval (list '_eval u))
                                           u expr))))))

            (if (null extraArgs) (progn (popc) right)
              (popc)
              (eval (list '_eval (cons right extraArgs))))))))))

(de sentinel (_Parm u expr)
  (if (vectorP _parm)
    (if (equal (_GetClass _parm) 'ObjectPointer)
      (if (NULL (_Getfield _parm 1))
        (progn
         (dale-error "argument not assinged a value" u self)
         (_NewInstance 'Error 1))
        (_Getfield _parm 1))
      _parm)
    _parm))

% SENTINEL checks for objects of type objectpointer and
% extracts the reverenced object if it is.  This is done
% inorder to make L-Valued and R-Valued objectpointer
% objects behave correctly.

% REMEMBER THIS BUG
% inorder to resolve all the instance variable references in a
% expression with in the same context a pre-evaluation scan of
% the expression must be done to resolve all the instance var
% references since during evaluation the context of subexpressions
% will change.  ResolvePass does this by placing a dalevar property
% on all instance variables referenced with the referenced object
% as the prop and returns the expression.

(de _ResolvePass (expr) % Returns a resolved expr
  (for (in u expr)
       (collect
        (cond 
         ((listP u) 
          (if (equal (first u) '_IVar) u %aready done.
            (_ResolvePass u))) % do nested expressions.
         ((numberP u) u)
         ((vectorP u) u)
%         (if (equal (_getclass u) 'code)
%           (_StoreField u 1 (_resolvePass (_GetField u 1)))
%           u))
         ((not (UnboundP u)) % a Var or Instance Var
          (if (numberP (eval u))
            (list '_IVar    % if var is a number it refers
                 (let ((_t (_instancevar
                            (get (_getClass self) 'daleid) u)))
                   (_GetField
                    self                     % to an Instance
                    (plus (first _t)
                          (minus (_FieldSize
                                  (get (first (cdr _t)) 'daleid)))
                          (_GetField self -1) -2))))
            u))
         (t (let ((temp (get u 'daleid)))
              (if (NULL temp)  % not a object but may be an inherited
                (if (NULL self)
                  u
                  (let ((_t (_instancevar % instance variable?
                             (get (_getclass self) 'daleid) u)))
                    (if (NULL _t)  % so check. if nil then it is not.
                      u
                      (list '_IVar % it is so make _IVar list
                       (_GetField 
                        self
                        (plus (first _t)
                              (minus (_FieldSize
                                      (get (first (cdr _t)) 'daleid)))
                              (_Getfield self -1) -2))))))
                u)))))))

(de !_MkProc ()
  % (printf "!_MkProc:%n")
  (setf _presentMethods
    (append _presentMethods (list _currentMethod)))
  (setf _currentMethod nil))

%--------------------- primitive implementation functions ------------
% Dale objects are represented by a vector.  The first element is its
% length.  The second is a reference to its class and the remaining
% elements are it's fields.

(de !_GetClass (objectPointer)
  (GETV objectPointer 1))
    %returns a pointer to the object's class %

(de !_Getfield (objectPointer fieldOffset) 
  (GETV objectPointer (PLUS fieldOffset 1)))
    % returns a pointer to the specified field of the object %
    % field indexing starts with index equal to 1 %

(de !_StoreField (objectPointer fieldOffset anObject)
  (PUTV objectPointer (PLUS fieldOffset 1) anObject)
  objectPointer)
    % returns the objectPointer after storing anObject in the specified
    % field.

(de !_Execute (objectPointer)
  (if (equal 'code (!_getclass objectPointer))
    (eval (!_getfield objectPointer 1))
    (printf "attemp to execute an non-code object%n")))
    % executes the code portition of a code object and returns its
    % resultant value.

(de !_NewInstance (classObjectPointer numberOfFields)
  (PROG (newpointer)
    (SETF newpointer (MAKE!-VECTOR (PLUS numberOfFields 1) nil))
    (PUTV newpointer 0 (PLUS numberOfFields 2))
          % Length field (0) is to be used for bounds checking and
          % storage management in other implementations.
    (PUTV newpointer 1 classObjectPointer)
          % Set up class field.
    (return newpointer)))
    % Creates a new object with class = classpointer and returns
    % a pointer to the new object.  The new object will
    % 'numberOfFields' fields in it.



% primitive functions used in the dale primitives package.

  % _FieldSize returns the size of the field required for object Object
  % by considering the size for it's instance variables and the size
  % required by it's chain of super classes variables.

(de _FieldSize (Object)
  (cond
   ((null object) 0)
   ((null (_GetField object 2)) (_FieldSize(get(_GetField object 1) 'daleid)))
   (t (plus2 (_FieldSize(get(_GetField object 1) 'daleid))
             (traverse object '(2 1 1))))))

(de _InstanceVar (Object name)
  (cond
   ((null object) nil)
   ((null (_GetField object 2))
    (_InstanceVar(get(_GetField object 1) 'daleid) name))
   (t (let ((q nil))
        (for (from i 1 (traverse object '(2 1 1)) 1)
             (do (if (equal name (traverse object '(2 (plus 1 i) 1)))
                   (setf q i))))
        (if (null q) (_InstanceVar(get(_GetField object 1) 'daleid)
                                  name)
          (list q (_GetField object 6)))))))


(de dale-error (msg token context)
  (if _errflg 
    nil
    (setf _errflg t)
    (if (null context) (setf context 'MONITOR)
      (setf context (_GetClass context)))
    (if (EQUAL 'Objectpointer context)
      (progn (setf context _contex)
             (while (equal 'Objectpointer (first context))
               (setf context (cdr context)))
             (if (null context) (setf context 'monitor)
               (setf context (first context)))))
    (if (null token)
      (printf "Dale - %w%n" msg)
      (printf "Dale - %w '%w'%n" msg token))
    (printf "    called by class %w%n" context))
  (_NewInstance 'ERROR 1))

(de rprint (object)
  (setf _errflg nil)
  (if _prtflg
    (print object)
    (let ((type (_GetClass object)))
      (cond
       ((equal type 'integer)
        (print (_GetField object 1)))
       ((equal type 'boolean)
        (print (if (equal (traverse '(1 1)) 0) 'false
                 'true)))
       (t nil))))
  nil)

(de _trace (expr context)
  (if _traceflg
    (progn
     (printf "    %w - " (car expr))
     (printf "%w%n" 
             (if (null context) 'monitor
               (if (equal (_getclass context) 'class)
                 (list 'class (_getField context 6))
                 (_getclass context)))))))8 ug> 